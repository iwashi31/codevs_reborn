#include "../common.h"
#include "beamSearchStrategy.h"

BeamSearchStrategy::State::State() = default;
BeamSearchStrategy::State::State(Field& field, int score) : field(field), score(score) {}
bool BeamSearchStrategy::State::operator<(const BeamSearchStrategy::State &a) const {
    return score > a.score;
}

BeamSearchStrategy::BeamSearchStrategy() : game(nullptr) {}

string BeamSearchStrategy::getName() {
    return "iwashiAI_v1.0";
}

Action BeamSearchStrategy::getAction(Game &game) {
    this->game = &game;

    return beamSearch(5, 300);
}

Action BeamSearchStrategy::beamSearch(int depth, unsigned width) {
    int turn = game->turn;
    vector<State> pool;
    pool.emplace_back(game->player[0].field, 0);
    rep(_, depth) {
        vector<State> nextPool;

        for (const auto &state : pool) {
            rep(position, FIELD_WIDTH - 1) rep(rotation, 4) {
                auto nextState = state;
                int chain = nextState.field.dropPack(game->packs[turn], position, rotation);
                if (chain == -1) continue;
                nextState.score = state.score + 1000 * CHAIN_SCORE[chain] + calcFieldScore(nextState.field);
                if (chain < 7 && chain > 0) nextState.score -= 100;
                nextState.actions.push_back(Action::createDropPackAction(position, rotation));
                nextState.chains.push_back(chain);
                nextPool.push_back(nextState);
            }
        }

        sort(all(nextPool));
        if (nextPool.size() > width) nextPool.resize(width);
        swap(pool, nextPool);
        turn++;
    }

    State& bestState = pool[0];
    Action& bestAction = bestState.actions[0];

    cerr << "---" << endl;
    cerr << "chains : ";
    for (int chain : bestState.chains) cerr << chain << "-";
    cerr << endl;
    cerr << "score  : " << bestState.score << endl;

    return bestAction;
}

int BeamSearchStrategy::calcFieldScore(Field& field) {
    int score = 0;

    // 連鎖に繋がりそうな位置関係の組み合わせがあれば加点
    rep(y, FIELD_HEIGHT) rep(x, FIELD_WIDTH) {
        if (field[y][x] == 0) continue;
        rep(i, 3) {
            int ix = x + DX_NEAR[i];
            int iy = y + DY_NEAR[i];
            if (!field.inField(ix, iy)) continue;
            if (field[y][x] + field[iy][ix] == ERASE_SUM) score++;
        }
    }

    return score;
}
