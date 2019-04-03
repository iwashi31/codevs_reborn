#ifndef CODEVS_REBORN_BEAMSEARCHSTRATEGY_H
#define CODEVS_REBORN_BEAMSEARCHSTRATEGY_H

#include "../common.h"

// 敵の状態は無視して、自分のスコアを最大化するように動く Strategy
// 中身はビームサーチ
class OnlyChainStrategy : public IStrategy {
private:
    struct State {
        Field field;
        long long score;
        vector<Action> actions;
        vector<int> chains;

        State();
        State(Field& field, int score);

        bool operator<(const State& a) const;
    };

    // 連鎖に繋がりそうな位置関係
    // 盤面の評価に使う
    const int DX_NEAR[3] = {1, 0, -1};
    const int DY_NEAR[3] = {2, 2, 2};

    Game* game;

    Action beamSearch(int depth, unsigned width);
    int calcFieldScore(Field& field);

public:
    OnlyChainStrategy();

    string getName() override;
    Action getAction(Game& game) override;
};

#endif //CODEVS_REBORN_BEAMSEARCHSTRATEGY_H