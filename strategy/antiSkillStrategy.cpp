#include "antiSkillStrategy.h"
#include "onlyChainStrategy.h"

AntiSkillStrategy::State::State() = default;
AntiSkillStrategy::State::State(Player& player, int score) : player(player), score(score) {}
bool AntiSkillStrategy::State::operator<(const AntiSkillStrategy::State &a) const {
    return score < a.score;
}

AntiSkillStrategy::AntiSkillStrategy() : game(nullptr) {}

string AntiSkillStrategy::getName() {
    return "iwashiAI_v3.0";
}

Action AntiSkillStrategy::getAction(Game &game) {
    this->game = &game;

    game.player[0].fallObstacles();

    State bestState;
    queue<State> q;
    q.push(State(game.player[0], 0));
    [&] () {
        rep(i, 3) {
            queue<State> nextQ;
            unordered_set<unsigned long long> pushedHash;
            while (!q.empty()) {
                auto prevState = q.front();
                q.pop();
                REP(position, 2, 7) rep(rotation, 4) {
                    auto state = prevState;
                    int chain = state.player.field.dropPack(game.packs[game.turn + i], position, rotation);
                    if (chain == -1) continue;
                    auto hash = state.player.field.getHash();
                    if (pushedHash.find(hash) != pushedHash.end()) continue;
                    pushedHash.insert(hash);
                    state.player.fallObstacles();
                    state.actions.push_back(Action::createDropPackAction(position, rotation));
                    state.chains.push_back(chain);
                    if (chain == 3) {
                        bestState = state;
                        return;
                    }
                    nextQ.push(state);
                }
            }
            q = nextQ;
        }
    }();

    cerr << "chains:";
    for (auto chain : bestState.chains) {
        cerr << chain << "_";
    }
    cerr << endl;
    if (!bestState.actions.empty() && bestState.chains.back() == 3) {
        return bestState.actions[0];
    }

    return OnlyChainStrategy().getAction(game);
}
