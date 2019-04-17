#include "randomStrategy.h"
#include "onlyChainStrategy.h"
#include "prioritizeSkillStrategy.h"
#include "antiSkillStrategy.h"
#include "switchableStrategy.h"

SwitchableStrategy::SwitchableStrategy() : game(nullptr) {}

string SwitchableStrategy::getName() {
    return "iwashiAI_v10.1";
}

Action SwitchableStrategy::getAction(Game &game) {
    this->game = &game;

    if (skillPrioritizeCheck()) {
        cerr << "use AntiSkill" << endl;
        onlyChainStrategy.clearQueue();
        return antiSkillStrategy.getAction(game);
    }

    cerr << "use OnlyChain" << endl;
    return onlyChainStrategy.getAction(game);
}

bool SwitchableStrategy::skillPrioritizeCheck() {
    return game->player[1].skillGage >= 48;
}
