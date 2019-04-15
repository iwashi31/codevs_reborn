#include "../common.h"
#include "onlyChainStrategy.h"
#include "randomStrategy.h"

OnlyChainStrategy::State::State() = default;
OnlyChainStrategy::State::State(Player& player, int score) : player(player), score(score) {}
bool OnlyChainStrategy::State::operator<(const OnlyChainStrategy::State &a) const {
    return score < a.score;
}

OnlyChainStrategy::OnlyChainStrategy() : game(nullptr) {}

string OnlyChainStrategy::getName() {
    return "iwashiAI_v1.14";
}

Action OnlyChainStrategy::getAction(Game &game) {
    this->game = &game;

    game.player[0].fallObstacles();

    int bestChain = 0;
    int bestChain1 = 0, bestChain2 = 0, bestChain3 = 0;
    unordered_set<unsigned long long> visitedHash[2];
    Action bestAction{};
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
            auto hash = player2.field.getHash();
            if (visitedHash[1].find(hash) != visitedHash[1].end()) continue;
            visitedHash[1].insert(hash);
            player2.fallObstacles();
            rep(pos3, FIELD_WIDTH - 1) rep(rot3, 4) {
                Player player3 = player2;
                int chain3 = player3.field.dropPack(game.packs[game.turn + 2], pos3, rot3);
                if (chain3 == -1) continue;
                int maxChain = max({chain1, chain2, chain3}) * 10;
                if (maxChain == chain1 * 10) maxChain += 9;
                else if (maxChain == chain2 * 10) maxChain += 8;
                else if (maxChain == chain3 * 10) maxChain += 7;
                if (maxChain > bestChain) {
                    bestChain = maxChain;
                    bestChain1 = chain1;
                    bestChain2 = chain2;
                    bestChain3 = chain3;
                    bestAction = Action::createDropPackAction(pos1, rot1);
                }
            }
        }
    }
    bestChain /= 10;
    cerr << "turn:" << game.turn << endl;
    cerr << "bestChain:" << bestChain1 << ", " << bestChain2 << ", " << bestChain3 << endl;
    if (bestChain >= 12) {
        return bestAction;
    }

    return chokudaiSearch(5, 0.3);
}

Action OnlyChainStrategy::chokudaiSearch(int depth, double timeLimit) {
    int turn = game->turn;
    if (turn + depth > MAX_TURN_NUM) {
        depth = MAX_TURN_NUM - turn;
    }

    Timer timer;
    vector<priority_queue<State>> q(static_cast<unsigned int>(depth + 1));
    vector<unordered_set<unsigned long long>> pushedHash(depth + 1);
    q[0].push(State(game->player[0], 0));
    int width = 0;
    while (timer.getTime() < timeLimit) {
        width++;
        rep(i, depth) {
            if (q[i].empty()) continue;
            State state = q[i].top(); q[i].pop();

            rep(position, FIELD_WIDTH - 1) rep(rotation, 4) {
                auto nextState = state;
                state.player.fallObstacles();
                int chain = nextState.player.field.dropPack(game->packs[turn + i], position, rotation);
                if (chain == -1) continue;

                unsigned long long hash = nextState.player.field.getHash();
                if (pushedHash[i + 1].find(hash) != pushedHash[i + 1].end()) continue;
                pushedHash[i + 1].insert(hash);

                nextState.score += calcFieldScore(nextState.player.field);
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
    score = 100 * score + rng.rand() % 100;

    return score;
}
