#include "../common.h"
#include "prioritizeSkillStrategy.h"
#include "randomStrategy.h"

PrioritizeSkillStrategy::State::State() = default;
PrioritizeSkillStrategy::State::State(Player& player, int score) : player(player), score(score) {}
bool PrioritizeSkillStrategy::State::operator<(const PrioritizeSkillStrategy::State &a) const {
    return score < a.score;
}

PrioritizeSkillStrategy::PrioritizeSkillStrategy() : game(nullptr) {}

string PrioritizeSkillStrategy::getName() {
    return "iwashiAI_v2.4";
}

Action PrioritizeSkillStrategy::getAction(Game &game) {
    this->game = &game;

    if (game.turn == 0) {
        bulkSearch(14, 18);
    }

    if (!actionQueue.empty()) {
        if (game.player[0].obstacleStock < 10) {
            auto action = actionQueue.front();
            actionQueue.pop();
            return action;
        } else {
            while (!actionQueue.empty()) actionQueue.pop();
        }
    }

    int explodeBlockNum = countExplodeBlockNum(game.player[0].field);
    if (searchType == SearchType::MAXIMIZE_EXPLODE_BLOCK_NUM) {
        if (game.player[0].skillGage >= SKILL_GAGE_THRESHOLD && explodeBlockNum >= 30) {
            return Action::createExplodeAction();
        } else if (game.player[0].skillGage < SKILL_GAGE_THRESHOLD) {
            searchType = SearchType ::INCREASE_GAGE;
        }
    } else if (searchType == SearchType::INCREASE_GAGE){
        if (game.player[0].skillGage >= SKILL_GAGE_THRESHOLD) {
            if (explodeBlockNum >= 30) {
                return Action::createExplodeAction();
            } else {
                searchType = SearchType ::MAXIMIZE_EXPLODE_BLOCK_NUM;
            }
        }
    } else assert(false);

    cerr << "---" << endl;
    cerr << "type :" << (searchType == SearchType::MAXIMIZE_EXPLODE_BLOCK_NUM ? 0 : 1) << endl;
    cerr << "turn:" << game.turn << " gage:" << game.player[0].skillGage << " num:" << explodeBlockNum << endl;

    return chokudaiSearch(10, 0.3);
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
                if (chain == -1) continue;
                nextState.score = calcFieldScore(nextState.player, chain);
                nextState.actions.push_back(Action::createDropPackAction(position, rotation));
                nextState.chains.push_back(chain);
                if (chain > 0) nextState.player.increaseSkillGage();
                q[i + 1].push(nextState);
            }
        }
    }

    if (q.back().empty()) {
        return RandomStrategy().getAction(*game);
    }

    State bestState = q.back().top();
    const Action& bestAction = bestState.actions[0];

    return bestAction;
}

void PrioritizeSkillStrategy::bulkSearch(int depth, double timeLimit) {
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
                if (chain == -1) continue;
                if (chain > 0) nextState.player.increaseSkillGage();
                nextState.score = calcFieldScore(nextState.player, chain);
                nextState.actions.push_back(Action::createDropPackAction(position, rotation));
                nextState.chains.push_back(chain);
                q[i + 1].push(nextState);
            }
        }
        if (q[depth].size() >= 50000) break;
    }

    if (q.back().empty()) {
        return;
    }

    State bestState = q.back().top();
    for (auto &action : bestState.actions) actionQueue.push(action);

    cerr << "gage:" << bestState.player.skillGage << " num:" << countExplodeBlockNum(bestState.player.field) << endl;
}

int PrioritizeSkillStrategy::calcFieldScore(Player& player, int chain) {
    Field& field = player.field;
    int score = 0;
    score = 100 * (countExplodeBlockNum(field) + min(player.skillGage, 80));
    if (chain > 1) {
        score -= 100000000;
    }
    score += field.countNumberBlock();
    return score;
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
