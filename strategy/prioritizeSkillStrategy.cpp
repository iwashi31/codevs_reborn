#include "../common.h"
#include "prioritizeSkillStrategy.h"

PrioritizeSkillStrategy::State::State() = default;
PrioritizeSkillStrategy::State::State(Player& player, int score) : player(player), score(score) {}
bool PrioritizeSkillStrategy::State::operator<(const PrioritizeSkillStrategy::State &a) const {
    return score < a.score;
}

PrioritizeSkillStrategy::PrioritizeSkillStrategy() : game(nullptr) {}

string PrioritizeSkillStrategy::getName() {
    return "iwashiAI_v2.0";
}

Action PrioritizeSkillStrategy::getAction(Game &game) {
    this->game = &game;

    int explodeBlockNum = countExplodeBlockNum(game.player[0].field);
    if (searchType == SearchType::MAXIMIZE_EXPLODE_BLOCK_NUM) {
        if (explodeBlockNum >= 30) {
            searchType = SearchType::INCREASE_SCORE;
        }
    } else if (searchType == SearchType::INCREASE_SCORE){
        if (game.player[0].skillGage >= SKILL_GAGE_THRESHOLD) {
            return Action::createExplodeAction();
        }
        if (explodeBlockNum <= 25) {
            searchType = SearchType::MAXIMIZE_EXPLODE_BLOCK_NUM;
        }
    } else assert(false);

    return chokudaiSearch(5, 0.1);
}

Action PrioritizeSkillStrategy::chokudaiSearch(int depth, double timeLimit) {
    int turn = game->turn;
    if (turn + depth > MAX_TURN_NUM) {
        depth = MAX_TURN_NUM - turn;
    }

    Timer timer;
    vector<priority_queue<State>> q(static_cast<unsigned>(depth + 1));
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
                nextState.score += calcFieldScore(nextState.player.field);
                if (searchType == SearchType::INCREASE_SCORE && chain > 0) {
                    nextState.score += INCREMENT_SKILL_GAGE;
                    nextState.score -= 5 * (chain - 1);
                }
                nextState.actions.push_back(Action::createDropPackAction(position, rotation));
                nextState.chains.push_back(chain);
                q[i + 1].push(nextState);
            }
        }
    }

    //const State& bestState = q.back().top();
    State bestState = q.back().top();
    const Action& bestAction = bestState.actions[0];

    cerr << "---" << endl;
    cerr << "type :" << (searchType == SearchType::MAXIMIZE_EXPLODE_BLOCK_NUM ? 0 : 1) << endl;
    cerr << "width:" << width << endl;
    cerr << "count:" << countExplodeBlockNum(bestState.player.field) << endl;

    return bestAction;
}

int PrioritizeSkillStrategy::calcFieldScore(Field& field) {
    return countExplodeBlockNum(field);
}

int PrioritizeSkillStrategy::countExplodeBlockNum(Field& field) {
    int cnt = 0;
    rep(y, FIELD_HEIGHT + PACK_SIZE) rep(x, FIELD_WIDTH) {
        if (field[y][x] == 0 || field[y][x] == OBSTACLE) continue;
        cnt += [&]() {
            for (int dy = -1; dy <= 1; dy++) for (int dx = -1; dx <= 1; dx++) {
                int nx = x + dx;
                int ny = y + dy;
                if (!field.inField(nx, ny)) continue;
                if (field[ny][nx] == EXPLODE_NUM) return 1;
            }
            return 0;
        }();
    }
    return cnt;
}
