#ifndef CODEVS_REBORN_SWITCHABLESTRATEGY_H
#define CODEVS_REBORN_SWITCHABLESTRATEGY_H

#include "../common.h"

class SwitchableStrategy : public IStrategy {
private:
    OnlyChainStrategy onlyChainStrategy;
    AntiSkillStrategy antiSkillStrategy;

    Game* game;

    // 大連鎖を放ったか否か
    bool isChained;
    vector<Game> gameHistory;

    bool skillPrioritizeCheck();

public:
    SwitchableStrategy();

    string getName() override;
    Action getAction(Game& game) override;
};
#endif //CODEVS_REBORN_SWITCHABLESTRATEGY_H
