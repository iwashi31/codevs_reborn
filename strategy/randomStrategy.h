#ifndef CODEVS_REBORN_RANDOMSTRATEGY_H
#define CODEVS_REBORN_RANDOMSTRATEGY_H

#include "../common.h"

class RandomStrategy : public IStrategy {
public:
    string getName() override;
    Action getAction(Game& game) override;
};

#endif //CODEVS_REBORN_RANDOMSTRATEGY_H
