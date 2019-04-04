#include "../common.h"
#include "onlyChainStrategy.h"

OnlyChainStrategy::State::State() = default;
OnlyChainStrategy::State::State(Player& player, int score) : player(player), score(score) {}
bool OnlyChainStrategy::State::operator<(const OnlyChainStrategy::State &a) const {
    return score < a.score;
}

OnlyChainStrategy::OnlyChainStrategy() : game(nullptr) {}

string OnlyChainStrategy::getName() {
    return "iwashiAI_v1.2";
}

Action OnlyChainStrategy::getAction(Game &game) {
    this->game = &game;

    game.player[0].fallObstacles();

    int bestChain = 0;
    Action bestAction{};
    rep(position, FIELD_WIDTH - 1) rep(rotation, 4) {
        Field field = game.player[0].field;
        int chain = field.dropPack(game.packs[game.turn], position, rotation);
        if (chain > bestChain) {
            bestChain = chain;
            bestAction = Action::createDropPackAction(position, rotation);
        }
    }
    cerr << "bestChain:" << bestChain << endl;
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

    State bestState = q.back().top();
    Action& bestAction = bestState.actions[0];

    return bestAction;
}

long long OnlyChainStrategy::calcFieldScore(Field& field) {
    int maxChain = 0;

    rep(y, FIELD_HEIGHT) rep(x, FIELD_WIDTH) {
        if (field[y][x] == 0 || field[y][x] == OBSTACLE) continue;
        if (field[y + 1][x] == 0) continue;
        if ((x > 1 && field[y + 1][x - 1] == 0 && (y < 2 || field[y - 2][x - 1] != 0))
            || (x < FIELD_WIDTH - 1 && field[y + 1][x + 1] == 0 && (y < 2 || field[y - 2][x + 1] != 0))) {
            Field tmpField = field;
            tmpField.update(x, y, 0);
            int chain = tmpField.drop();
            maxChain = max(maxChain, chain);
        }
    }

    return CHAIN_SCORE[maxChain];
}
