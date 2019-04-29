#include "../common.h"
#include "chainStrategy.h"
#include "randomStrategy.h"

ChainStrategy::State::State() = default;
ChainStrategy::State::State(Player& player, int score) : player(player), score(score) {}
bool ChainStrategy::State::operator<(const ChainStrategy::State &a) const {
    return score < a.score;
}

ChainStrategy::ChainStrategy() : game(nullptr), bulkSearchFlag(true), noBulkCount(0), prevObstacleStock(0), bulkSearchCount(0), stackedBlockLines(0) {}
ChainStrategy::ChainStrategy(bool bulkSearchFlag) : game(nullptr), bulkSearchFlag(bulkSearchFlag), noBulkCount(0), prevObstacleStock(0), bulkSearchCount(0), stackedBlockLines(0) {}

string ChainStrategy::getName() {
    return "iwashiAI_v1.45";
}

Action ChainStrategy::getAction(Game &game) {
    logger.printLine("call OnlyChain");

    this->game = &game;

    //if (game.turn == 1) {
    //    Timer timer;
    //    while (timer.getTime() < 2.0) continue;
    //}

    if (game.player[0].obstacleStock >= 10) stackedBlockLines++;

    if ((game.player[0].obstacleStock / 10 > prevObstacleStock / 10)
        || (prevObstacleStock / 10 > 0 && game.player[0].obstacleStock / 10 == prevObstacleStock / 10)) {
        bulkSearchFlag = true;
        clearQueue();
    }

    prevObstacleStock = game.player[0].obstacleStock;

    const int lowerChainNum = game.turn == 0 ? 13 : 11;

    if (bulkSearchFlag) {
        bulkSearchFlag = false;
        bulkSearch(15, lowerChainNum, min(17, max(10, 18 - 4 * bulkSearchCount)));
    } else if (actionQueue.size() == 4) {
        const int depth = 4;
        vector<vector<State>> states(depth + 1);
        vector<unordered_set<unsigned long long>> pushedHash(static_cast<unsigned int>(depth + 1));
        vector<State> statePool;
        states[0].push_back(State(game.player[0], 0));
        rep(i, depth) {
            for (auto &state : states[i]) {
                state.player.fallObstacles();
                rep(position, FIELD_WIDTH - 1) rep(rotation, 4) {
                    auto nextState = state;
                    auto chainInfo = nextState.player.field.dropPackWithInfo(game.packs[game.turn + i], position, rotation);
                    if (chainInfo.chainNum == -1) continue;

                    nextState.actions.push_back(Action::createDropPackAction(position, rotation));
                    nextState.chains.push_back(chainInfo.chainNum);

                    if (chainInfo.chainNum >= (bulkSearchCount <= 1 ? 13 : 11)) {
                        nextState.chainInfo = chainInfo;
                        statePool.push_back(nextState);
                        continue;
                    }

                    if (i < depth - 1) states[i + 1].push_back(nextState);
                }
            }
        }

        assert(!statePool.empty());

        sort(all(statePool), [&](State &s1, State &s2) {
            if (s1.chainInfo.chainNum >= lowerChainNum && s2.chainInfo.chainNum < lowerChainNum) return true;
            if (s2.chainInfo.chainNum >= lowerChainNum && s1.chainInfo.chainNum < lowerChainNum) return false;

            int val1 = (2 * s1.chainInfo.chainNum - 3 * s1.actions.size()) * 2 + s1.chainInfo.robustNum - s1.player.obstacleStock * 8 / 10;
            int val2 = (2 * s2.chainInfo.chainNum - 3 * s2.actions.size()) * 2 + s2.chainInfo.robustNum - s2.player.obstacleStock * 8 / 10;
            if (game.turn == 0) {
                val1 -= s1.chainInfo.robustNum <= 1 ? 100 : 0;
                val2 -= s2.chainInfo.robustNum <= 1 ? 100 : 0;
            }
            if (val1 == val2) {
                if (s1.actions.size() - s1.chainInfo.robustNum == s2.actions.size() - s2.chainInfo.robustNum) {
                    int b1 = s1.player.field.countNumberBlock();
                    int b2 = s2.player.field.countNumberBlock();
                    if (b1 == b2) {
                        return s1.chainInfo.robustNum > s2.chainInfo.robustNum;
                    }
                    return b1 > b2;
                }
                return s1.actions.size() - s1.chainInfo.robustNum < s2.actions.size() - s2.chainInfo.robustNum;
            }
            return val1 > val2;
        });
        State bestState = statePool[0];

        logger.print("  turn:");
        logger.print(bestState.actions.size());
        logger.print("  chn:");
        logger.print(bestState.chainInfo.chainNum);
        logger.print("  lft:");
        logger.print(bestState.player.field.countNumberBlock());
        logger.print("  rbst:");
        logger.printLine(bestState.chainInfo.robustNum);

        clearQueue();
        for (auto &action : bestState.actions) actionQueue.push(action);
    }

    if (!actionQueue.empty()) {
        logger.print(" q:");
        rep(i, actionQueue.size()) logger.print("*");
        logger.printLine();

        Action action = actionQueue.front();
        actionQueue.pop();
        if (actionQueue.empty()) bulkSearchFlag = true;
        return action;
    }

    int bestChain = 0;
    int bestChain1 = 0, bestChain2 = 0, bestChain3 = 0;
    int bestBlockLeft = 1234567;
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
    logger.printLine(" brute force");
    logger.print("  bestChain:");
    logger.print(bestChain1);
    logger.print(", ");
    logger.print(bestChain2);
    logger.print(", ");
    logger.printLine(bestChain3);
    if (bestChain >= 10) {
        actionQueue.push(bestAction2);
        actionQueue.push(bestAction3);
    }
    if (bestChain >= 8) {
        if (bestChain == bestChain1) bulkSearchFlag = true;
        return bestAction;
    }

    noBulkCount++;
    if (noBulkCount >= 3) {
        bulkSearchFlag = true;
        noBulkCount = 0;
    }

    return singleSearch(5, 0.6);
}

int ChainStrategy::getQueueSize() {
    return actionQueue.size();
}

void ChainStrategy::clearQueue() {
    while (!actionQueue.empty()) actionQueue.pop();
}

Action ChainStrategy::singleSearch(int depth, double timeLimit) {
    int turn = game->turn;
    if (turn + depth > MAX_TURN_NUM) {
        depth = MAX_TURN_NUM - turn;
    }

    vector<bool> allowErase(10);
    rep(i, 10) allowErase[i] = true;

    int startX, endX;
    if (stackedBlockLines == 0) {
        startX = 2;
        endX = 7;
    } else {
        startX = 0;
        endX = FIELD_WIDTH - 1;
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

            REP(position, startX, endX) rep(rotation, 4) {
                auto nextState = state;
                nextState.player.fallObstacles();
                int chain = nextState.player.field.dropPack(game->packs[turn + i], position, rotation);
                if (chain == -1) continue;

                unsigned long long hash = nextState.player.field.getHash();
                if (pushedHash[i + 1].find(hash) != pushedHash[i + 1].end()) continue;
                pushedHash[i + 1].insert(hash);

                nextState.score += calcFieldScore(nextState.player.field, allowErase);
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

void ChainStrategy::bulkSearch(int depth, int minChain, double timeLimit) {
    logger.printLine(" bulk search!");

    clearQueue();

    bulkSearchCount++;

    vector<bool> allowErase(10);
    if (game->turn > 0) {
        rep(i, 10) allowErase[i] = true;
    } else {
        rep(i, 10) allowErase[i] = false;
        REP(t, 10, 13) {
            rep(y, PACK_SIZE) rep(x, PACK_SIZE) allowErase[10 - game->packs[t][y][x]] = true;
        }
    }

    int startX, endX;
    if (stackedBlockLines == 0) {
        startX = 1;
        endX = 6;
    } else {
        startX = 0;
        endX = FIELD_WIDTH - 1;
    }

    const int lowerChainNum = minChain;

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

            REP(position, startX, endX) rep(rotation, 4) {
                if (game->turn == 0 && i <= depth / 2 && (rng.rand() & 3) == 0) continue;

                auto nextState = state;
                nextState.player.fallObstacles();
                ChainInfo chainInfo = nextState.player.field.dropPackWithInfo(game->packs[game->turn + i], position, rotation);
                if (chainInfo.chainNum == -1) continue;
                if (chainInfo.chainNum >= 2 && chainInfo.chainNum < lowerChainNum) continue;

                nextState.score += calcFieldScore(nextState.player.field, allowErase);
                nextState.actions.push_back(Action::createDropPackAction(position, rotation));
                nextState.chains.push_back(chainInfo.chainNum);

                if (chainInfo.chainNum >= 11) {
                    nextState.chainInfo = chainInfo;
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

    // logger.printLine("width:" + to_string(width));

    if (statePool.empty()) {
        logger.printLine("  failed");
        return;
    }

    sort(all(statePool), [&](State &s1, State &s2) {
        if (s1.chainInfo.chainNum >= lowerChainNum && s2.chainInfo.chainNum < lowerChainNum) return true;
        if (s2.chainInfo.chainNum >= lowerChainNum && s1.chainInfo.chainNum < lowerChainNum) return false;

        int val1 = (2 * s1.chainInfo.chainNum - 3 * s1.actions.size()) * 2 + s1.chainInfo.robustNum - s1.player.obstacleStock * 8 / 10;
        int val2 = (2 * s2.chainInfo.chainNum - 3 * s2.actions.size()) * 2 + s2.chainInfo.robustNum - s2.player.obstacleStock * 8 / 10;
        if (game->turn == 0) {
            val1 -= s1.chainInfo.robustNum <= 1 ? 100 : 0;
            val2 -= s2.chainInfo.robustNum <= 1 ? 100 : 0;
        }
        if (val1 == val2) {
            if (s1.actions.size() - s1.chainInfo.robustNum == s2.actions.size() - s2.chainInfo.robustNum) {
                int b1 = s1.player.field.countNumberBlock();
                int b2 = s2.player.field.countNumberBlock();
                if (b1 == b2) {
                    return s1.chainInfo.robustNum > s2.chainInfo.robustNum;
                }
                return b1 > b2;
            }
            return s1.actions.size() - s1.chainInfo.robustNum < s2.actions.size() - s2.chainInfo.robustNum;
        }
        return val1 > val2;
    });

    noBulkCount = 0;

    State bestState = statePool[0];

    logger.print("  turn:");
    logger.print(bestState.actions.size());
    logger.print("  chn:");
    logger.print(bestState.chainInfo.chainNum);
    logger.print("  lft:");
    logger.print(bestState.player.field.countNumberBlock());
    logger.print("  rbst:");
    logger.printLine(bestState.chainInfo.robustNum);

    for (auto &action : bestState.actions) actionQueue.push(action);
}

long long ChainStrategy::calcFieldScore(Field& field, vector<bool> &allowErase) {
    int maxChain = 0;

    int emptySideNum = 0;
    int maxHeight = 0;
    rep(y, FIELD_HEIGHT) rep(x, FIELD_WIDTH) {
        if (field[y][x] == 0) continue;
        maxHeight = max(maxHeight, y);
        if (field[y][x] == OBSTACLE) continue;
        if (x > 0 && field[y][x - 1] != 0) emptySideNum++;
        if (x < FIELD_WIDTH - 1 && field[y][x + 1] != 0) emptySideNum++;
        if (!allowErase[field[y][x]]) continue;
        if (field[y + 1][x] == 0) continue;
        if ((x > 1 && field[y + 1][x - 1] == 0)
            || (x < FIELD_WIDTH - 1 && field[y + 1][x + 1] == 0)) {
            Field tmpField = field;
            tmpField.update(x, y, 0);
            int chain = tmpField.drop();
            maxChain = max(maxChain, chain);
        }
    }

    long long score = CHAIN_SCORE[maxChain] - max(0, maxHeight - 10) + emptySideNum;
    score = 10000 * score + field.countNumberBlock() * 100 + rng.rand() % 100;

    return score;
}
