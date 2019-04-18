#include "../common.h"
#include "onlyChainStrategy.h"
#include "randomStrategy.h"

OnlyChainStrategy::State::State() = default;
OnlyChainStrategy::State::State(Player& player, int score) : player(player), score(score) {}
bool OnlyChainStrategy::State::operator<(const OnlyChainStrategy::State &a) const {
    return score < a.score;
}

OnlyChainStrategy::OnlyChainStrategy() : game(nullptr), bulkSearchFlag(true), noBulkCount(0), prevObstacleStock(0) {}

string OnlyChainStrategy::getName() {
    return "iwashiAI_v1.22";
}

Action OnlyChainStrategy::getAction(Game &game) {
    this->game = &game;

    if (prevObstacleStock >= 10 && game.player[0].obstacleStock < 10) {
        bulkSearchFlag = true;
    }

    prevObstacleStock = game.player[0].obstacleStock;

    if (game.player[0].obstacleStock < 10 && bulkSearchFlag) {
        bulkSearchFlag = false;
        if (game.turn == 0) return bulkSearch(15, 18);
        return bulkSearch(15, 5);
    }

    if (!actionQueue.empty()) {
        if (game.player[0].obstacleStock < 10) {
            Action action = actionQueue.front();
            actionQueue.pop();
            if (actionQueue.empty()) bulkSearchFlag = true;
            return action;
        } else {
            clearQueue();
        }
    }

    int bestChain = 0;
    int bestChain1 = 0, bestChain2 = 0, bestChain3 = 0;
    int bestBlockLeft = INT_MAX;
    unordered_set<unsigned long long> visitedHash[2];
    Action bestAction{}, bestAction2{}, bestAction3{};
    Player tmpPlayer = game.player[0];
    tmpPlayer.fallObstacles();
    rep(pos1, FIELD_WIDTH - 1) rep(rot1, 4) {
        Player player = tmpPlayer;
        int chain1 = player.field.dropPack(game.packs[game.turn], pos1, rot1);
        if (chain1 == -1) continue;
        auto hash = player.field.getHash();
        if (visitedHash[0].find(hash) != visitedHash[0].end()) continue;
        visitedHash[0].insert(hash);
        player.fallObstacles();
        rep(pos2, FIELD_WIDTH - 1) rep(rot2, 4) {
            Player player2 = player;
            int chain2 = player2.field.dropPack(game.packs[game.turn + 1], pos2, rot2);
            if (chain2 == -1) continue;
            auto hash2 = player2.field.getHash();
            if (visitedHash[1].find(hash2) != visitedHash[1].end()) continue;
            visitedHash[1].insert(hash2);
            player2.fallObstacles();
            rep(pos3, FIELD_WIDTH - 1) rep(rot3, 4) {
                Player player3 = player2;
                int chain3 = player3.field.dropPack(game.packs[game.turn + 2], pos3, rot3);
                if (chain3 == -1) continue;
                int maxChain = max({chain1, chain2, chain3}) * 10;
                if (maxChain == chain1 * 10) maxChain += 9;
                else if (maxChain == chain2 * 10) maxChain += 8;
                else if (maxChain == chain3 * 10) maxChain += 7;
                int blockLeft = player.field.countNumberBlock();
                if (maxChain > bestChain || (maxChain == bestChain && blockLeft > bestBlockLeft)) {
                    bestChain = maxChain;
                    bestChain1 = chain1;
                    bestChain2 = chain2;
                    bestChain3 = chain3;
                    bestAction = Action::createDropPackAction(pos1, rot1);
                    bestAction2 = Action::createDropPackAction(pos2, rot2);
                    bestAction3 = Action::createDropPackAction(pos3, rot3);
                    bestBlockLeft = blockLeft;
                }
            }
        }
    }
    bestChain /= 10;
    cerr << "turn:" << game.turn << endl;
    cerr << "bestChain:" << bestChain1 << ", " << bestChain2 << ", " << bestChain3 << endl;
    if (bestChain >= 14) {
        actionQueue.push(bestAction2);
        actionQueue.push(bestAction3);
    }
    if (bestChain >= (game.player[0].field.countBlock() <= 100 ? 12 : 9)) {
        if (bestChain == bestChain1) bulkSearchFlag = true;
        return bestAction;
    }

    noBulkCount++;
    if (noBulkCount >= 5) {
        bulkSearchFlag = true;
        noBulkCount = 0;
    }

    return singleSearch(5, 0.6);
}

void OnlyChainStrategy::clearQueue() {
    while (!actionQueue.empty()) actionQueue.pop();
}

Action OnlyChainStrategy::singleSearch(int depth, double timeLimit) {
    int turn = game->turn;
    if (turn + depth > MAX_TURN_NUM) {
        depth = MAX_TURN_NUM - turn;
    }

    Timer timer;
    vector<priority_queue<State>> q(static_cast<unsigned int>(depth + 1));
    vector<unordered_set<unsigned long long>> pushedHash(static_cast<unsigned int>(depth + 1));
    q[0].push(State(game->player[0], 0));
    int width = 0;
    while (timer.getTime() < timeLimit) {
        width++;
        rep(i, depth) {
            if (q[i].empty()) continue;
            State state = q[i].top(); q[i].pop();

            REP(position, 2, 7) rep(rotation, 4) {
                auto nextState = state;
                state.player.fallObstacles();
                int chain = nextState.player.field.dropPack(game->packs[turn + i], position, rotation);
                if (chain == -1) continue;

                unsigned long long hash = nextState.player.field.getHash();
                if (pushedHash[i + 1].find(hash) != pushedHash[i + 1].end()) continue;
                pushedHash[i + 1].insert(hash);

                nextState.score += calcFieldScore(nextState.player.field);
                if (position < 2 || position > 7) nextState.score -= 1000;
                nextState.actions.push_back(Action::createDropPackAction(position, rotation));
                nextState.chains.push_back(chain);
                q[i + 1].push(nextState);
            }
        }
    }

    if (q.back().empty()) {
        return RandomStrategy().getAction(*game);
    }

    State bestState = q.back().top();
    Action& bestAction = bestState.actions[0];

    return bestAction;
}

Action OnlyChainStrategy::bulkSearch(int depth, double timeLimit) {
    cerr << "bulk search!" << endl;

    Timer timer;
    vector<set<State>> q(static_cast<unsigned int>(depth + 1));
    vector<unordered_set<unsigned long long>> pushedHash(static_cast<unsigned int>(depth + 1));
    q[0].insert(State(game->player[0], 0));
    int width = 0;
    vector<State> statePool;
    while (timer.getTime() < timeLimit) {
        width++;
        rep(i, depth) {
            if (q[i].empty()) continue;
            auto it = q[i].end(); it--;
            State state = *it; q[i].erase(it);

            REP(position, 2, 7) rep(rotation, 4) {
                auto nextState = state;
                nextState.player.fallObstacles();
                int chain = nextState.player.field.dropPack(game->packs[game->turn + i], position, rotation);
                if (chain == -1) continue;
                if (chain >= 2 && chain < 12) continue;

                nextState.score += calcFieldScore(nextState.player.field);
                nextState.actions.push_back(Action::createDropPackAction(position, rotation));
                nextState.chains.push_back(chain);

                if (chain >= 12) {
                    statePool.push_back(nextState);
                    continue;
                }

                if (i == depth - 1) continue;

                unsigned long long hash = nextState.player.field.getHash();
                if (pushedHash[i + 1].find(hash) != pushedHash[i + 1].end()) continue;
                pushedHash[i + 1].insert(hash);

                q[i + 1].insert(nextState);
                if (q[i + 1].size() > 10000) {
                    q[i + 1].erase(q[i + 1].begin());
                }
            }
        }
    }

    if (statePool.empty()) {
        //if (game->turn == 0) {
        //    ofstream fout("chain.log", ios::app);
        //    fout << "-1 -1" << endl;
        //}
        return RandomStrategy().getAction(*game);
    }

    sort(all(statePool), [](State &s1, State &s2) {
        int val1 = s1.chains.back() - s1.actions.size();
        int val2 = s2.chains.back() - s2.actions.size();
        if (val1 == val2) {
            if (s1.actions.size() == s2.actions.size()) {
                int b1 = s1.player.field.countNumberBlock();
                int b2 = s2.player.field.countNumberBlock();
                return b1 > b2;
            }
            return s1.actions.size() < s2.actions.size();
        }
        return val1 > val2;
    });

    noBulkCount = 0;

    State bestState = statePool[0];
    Action& bestAction = bestState.actions[0];

    cerr << "turn:" << game->turn + bestState.actions.size();
    cerr << "chain:" << bestState.chains.back() << endl;
    //if (game->turn == 0) {
    //    ofstream fout("chain.log", ios::app);
    //    fout << bestState.actions.size() << " " << bestState.chains.back() << endl;
    //}

    clearQueue();
    for (auto &action : bestState.actions) actionQueue.push(action);
    actionQueue.pop();

    return bestAction;
}

long long OnlyChainStrategy::calcFieldScore(Field& field) {
    int maxChain = 0;

    int maxHeight = 0;
    rep(y, FIELD_HEIGHT) rep(x, FIELD_WIDTH) {
        if (field[y][x] == 0) continue;
        maxHeight = max(maxHeight, y);
        if (field[y][x] == OBSTACLE) continue;
        if (field[y + 1][x] == 0) continue;
        if ((x > 1 && field[y + 1][x - 1] == 0)
            || (x < FIELD_WIDTH - 1 && field[y + 1][x + 1] == 0)) {
            Field tmpField = field;
            tmpField.update(x, y, 0);
            int chain = tmpField.drop();
            maxChain = max(maxChain, chain);
        }
    }

    long long score = CHAIN_SCORE[maxChain] - max(0, maxHeight - 10);
    score = 10000 * score + field.countNumberBlock() * 100 + rng.rand() % 100;

    return score;
}
