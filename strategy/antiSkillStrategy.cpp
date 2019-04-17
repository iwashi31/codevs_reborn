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
    return "iwashiAI_v3.2";
}

Action AntiSkillStrategy::getAction(Game &game) {
    this->game = &game;

    if (game.player[0].skillGage >= SKILL_GAGE_THRESHOLD) {
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
        int c1 = 0, c2 = 0;
        rep(i, s1.chains.size()) {
            if (s1.chains[i] == 3) {
                t1 = min(t1, i);
                c1++;
            }
        }
        rep(i, s2.chains.size()) {
            if (s2.chains[i] == 3) {
                t2 = min(t2, i);
                c2++;
            }
        }
        if (t1 == t2) return c1 > c2;
        return t1 < t2;
    });

    if (q.empty()) {
        return RandomStrategy().getAction(game);
    }

    State bestState = q[0];

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
