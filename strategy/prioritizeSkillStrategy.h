#ifndef CODEVS_REBORN_CHOKUDAISEARCHSTRATEGY_H
#define CODEVS_REBORN_CHOKUDAISEARCHSTRATEGY_H

#include "../common.h"

// 敵の状態は無視して、自分のスコアを最大化するように動く Strategy
// 中身はビームサーチ
class PrioritizeSkillStrategy : public IStrategy {
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

    enum class SearchType {
        MAXIMIZE_EXPLODE_BLOCK_NUM,
        INCREASE_GAGE
    } searchType = SearchType::INCREASE_GAGE;

    Game* game;

    Action chokudaiSearch(int depth, double timeLimit);
    int calcFieldScore(Field& field, int chain);
    int countExplodeBlockNum(Field& field);

public:
    PrioritizeSkillStrategy();

    string getName() override;
    Action getAction(Game& game) override;
};

#endif //CODEVS_REBORN_CHOKUDAISEARCHSTRATEGY_H
