#include "../common.h"
#include "onlyChainStrategy.h"

OnlyChainStrategy::State::State() = default;
OnlyChainStrategy::State::State(Player& player, int score) : player(player), score(score) {}
bool OnlyChainStrategy::State::operator<(const OnlyChainStrategy::State &a) const {
    return score < a.score;
}

OnlyChainStrategy::OnlyChainStrategy() : game(nullptr) {}

string OnlyChainStrategy::getName() {
    return "iwashiAI_v1.5";
}

Action OnlyChainStrategy::getAction(Game &game) {
    this->game = &game;

    game.player[0].fallObstacles();

    int bestChain = 0;
    int bestChain1 = 0, bestChain2 = 0;
    Action bestAction{}, bestAction2{};
    rep(pos1, FIELD_WIDTH - 1) rep(rot1, 4) {
        Player player = game.player[0];
        player.fallObstacles();
        int chain1 = player.field.dropPack(game.packs[game.turn], pos1, rot1);
        if (chain1 == -1) continue;
        rep(pos2, FIELD_WIDTH - 1) rep(rot2, 4) {
            Player player2 = player;
            player2.fallObstacles();
            int chain2 = player2.field.dropPack(game.packs[game.turn + 1], pos2, rot2);
            if (chain2 == -1) continue;
            if (max(chain1, chain2 - 1) > bestChain) {
                bestChain = max(chain1, chain2 - 1);
                bestChain1 = chain1;
                bestChain2 = chain2;
                bestAction = Action::createDropPackAction(pos1, rot1);
                bestAction2 = Action::createDropPackAction(pos2, rot2);
            }
        }
    }
    cerr << "turn:" << game.turn << endl;
    cerr << "bestChain:" << bestChain1 << ", " << bestChain2 << endl;
    cerr << "action:(" << bestAction.position << ", " << bestAction.rotation << "), (" << bestAction2.position << ", " << bestAction2.rotation << ")" << endl;
    if (bestChain >= 15) {
        return bestAction;
    }

    return chokudaiSearch(10, 0.3);
}

Action OnlyChainStrategy::chokudaiSearch(int depth, double timeLimit) {
    int turn = game->turn;
    if (turn + depth > MAX_TURN_NUM) {
        depth = MAX_TURN_NUM - turn;
    }

    Timer timer;
    vector<priority_queue<State>> q(static_cast<unsigned int>(depth + 1));
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
                nextState.score += calcFieldScore(nextState.player.field);
                nextState.actions.push_back(Action::createDropPackAction(position, rotation));
                nextState.chains.push_back(chain);
                q[i + 1].push(nextState);
            }
        }
    }

    if (q.back().empty()) {
        return Action::createDropPackAction(-1, -1);
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

    return CHAIN_SCORE[maxChain] - max(0, maxHeight - 10);
}
