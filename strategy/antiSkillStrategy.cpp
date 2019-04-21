#include "randomStrategy.h"
#include "antiSkillStrategy.h"
#include "onlyChainStrategy.h"

AntiSkillStrategy::State::State() = default;
AntiSkillStrategy::State::State(Player& player, int score) : player(player), score(score) {}
bool AntiSkillStrategy::State::operator<(const AntiSkillStrategy::State &a) const {
    return score < a.score;
}

AntiSkillStrategy::AntiSkillStrategy() : game(nullptr) {}

string AntiSkillStrategy::getName() {
    return "iwashiAI_v3.4";
}

Action AntiSkillStrategy::getAction(Game &game) {
    cerr << "call AntiSkill" << endl;

    this->game = &game;

    if (game.player[0].skillGage >= SKILL_GAGE_THRESHOLD && game.player[0].field.countExplodeBlockNum() > 0) {
        return Action::createExplodeAction();
    }

    game.player[0].fallObstacles();

    vector<State> q;
    q.push_back(State(game.player[0], 0));
    rep(i, 3) {
        vector<State> nextQ;
        unordered_set<unsigned long long> pushedHash;
        for (auto &prevState : q) {
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
                nextQ.push_back(state);
            }
        }
        q = nextQ;
    }
    sort(all(q), [](State &s1, State &s2) {
        int t1 = 3, t2 = 3;
        int a1 = 0, a2 = 0;
        rep(i, s1.chains.size()) {
            if (s1.chains[i] == 3) {
                t1 = min(t1, i);
                a1--;
            } else if (s1.chains[i] < 3) {
                a1 += s1.chains[i];
            }
        }
        rep(i, s2.chains.size()) {
            if (s2.chains[i] == 3) {
                t2 = min(t2, i);
                a2--;
            } else if (s2.chains[i] < 3) {
                a2 += s2.chains[i];
            }
        }
        if (t1 == t2) return a1 < a2;
        return t1 < t2;
    });

    if (q.empty()) {
        return RandomStrategy().getAction(game);
    }

    State bestState = q[0];

    cerr << " chains:";
    for (auto chain : bestState.chains) {
        cerr << chain << "_";
    }
    cerr << endl;
    if (!bestState.actions.empty() && *max_element(all(bestState.chains)) >= 3) {
        return bestState.actions[0];
    }

    return OnlyChainStrategy(false).getAction(game);
}
