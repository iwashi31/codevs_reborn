#ifndef CODEVS_REBORN_SKILLSTRATEGY_H
#define CODEVS_REBORN_SKILLSTRATEGY_H

#include "../common.h"

// 敵の状態は無視して、自分のスコアを最大化するように動く Strategy
// 中身はビームサーチ
class SkillStrategy : public IStrategy {
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

    enum class SearchType {
        MAXIMIZE_EXPLODE_BLOCK_NUM,
        INCREASE_GAGE
    } searchType = SearchType::INCREASE_GAGE;

    Game* game;
    queue<Action> actionQueue;

    Action chokudaiSearch(int depth, double timeLimit);
    void bulkSearch(int depth, double timeLimit);
    int calcFieldScore(Player& player, int chain);

public:
    SkillStrategy();

    string getName() override;
    Action getAction(Game& game) override;
};

#endif //CODEVS_REBORN_SKILLSTRATEGY_H
