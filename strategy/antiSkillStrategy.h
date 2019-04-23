#ifndef CODEVS_REBORN_ANTISKILLSTRATEGY_H
#define CODEVS_REBORN_ANTISKILLSTRATEGY_H

#include "../common.h"

class AntiSkillStrategy : public IStrategy {
private:
    struct State {
        Player player;
        long long score;
        vector<Action> actions;
        vector<int> chains;

        State();
        State(Player& player, int score);

        bool operator<(const State& a) const;
    };

    Logger logger;

    Game* game;

public:
    AntiSkillStrategy();

    string getName() override;
    Action getAction(Game& game) override;
};

#endif //CODEVS_REBORN_ANTISKILLSTRATEGY_H
