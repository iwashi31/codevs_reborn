#ifndef CODEVS_REBORN_SWITCHABLESTRATEGY_H
#define CODEVS_REBORN_SWITCHABLESTRATEGY_H

#include "../common.h"

class SwitchableStrategy : public IStrategy {
private:
    struct State {
        Player player;
        long long score;
        vector<Action> actions;
        vector<int> chains;
        ChainInfo chainInfo;

        State();
        State(Player& player, int score);

        bool operator<(const State& a) const;
    };

    Logger logger;

    OnlyChainStrategy onlyChainStrategy;
    AntiSkillStrategy antiSkillStrategy;

    Game* game;

    // 大連鎖を放ったか否か
    bool isChained;
    bool isChainedEnemy;
    vector<Game> gameHistory;

    bool skillPrioritizeCheck();
    void analyze();

public:
    SwitchableStrategy();

    string getName() override;
    Action getAction(Game& game) override;
};
#endif //CODEVS_REBORN_SWITCHABLESTRATEGY_H
