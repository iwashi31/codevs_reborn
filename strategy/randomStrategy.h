#ifndef CODEVS_REBORN_RANDOMSTRATEGY_H
#define CODEVS_REBORN_RANDOMSTRATEGY_H

#include "../common.h"

class RandomStrategy : public IStrategy {
public:
    string getName() override {
        return "IwashiRandomAI";
    }

    Action getAction(Game& game) override {
        auto& me = game.player[0];
        if (me.skillGage >= SKILL_GAGE_THRESHOLD) {
            return Action::createExplodeAction();
        }
        int position = (int) (randXor() % (FIELD_WIDTH - 1));
        int rotation = (int) (randXor() % 4);
        return Action::createDropPackAction(position, rotation);
    }
};

#endif //CODEVS_REBORN_RANDOMSTRATEGY_H
