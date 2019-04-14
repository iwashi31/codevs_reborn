#include "../common.h"
#include "randomStrategy.h"

string RandomStrategy::getName() {
    return "IwashiRandomAI";
}

Action RandomStrategy::getAction(Game &game) {
    auto& me = game.player[0];
    if (me.skillGage >= SKILL_GAGE_THRESHOLD) {
        return Action::createExplodeAction();
    }
    int position = static_cast<int>(rng.rand() % (FIELD_WIDTH - 1));
    int rotation = static_cast<int>(rng.rand() % 4);
    return Action::createDropPackAction(position, rotation);
}
