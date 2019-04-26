#ifndef CODEVS_REBORN_CHAINSTRATEGY_H
#define CODEVS_REBORN_CHAINSTRATEGY_H

#include "../common.h"

// 敵の状態は無視して、自分のスコアを最大化するように動く Strategy
// 中身はビームサーチ
class ChainStrategy : public IStrategy {
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

    // 連鎖に繋がりそうな位置関係
    // 盤面の評価に使う
    const int DX_NEAR[3] = {1, 0, -1};
    const int DY_NEAR[3] = {2, 2, 2};

    Game* game;

    int bulkSearchCount;
    int stackedBlockLines;
    int prevObstacleStock;
    bool bulkSearchFlag;
    int noBulkCount;
    queue<Action> actionQueue;

    Action singleSearch(int depth, double timeLimit);
    void bulkSearch(int depth, int minChain, double timeLimit);
    long long calcFieldScore(Field& field, vector<bool> &allowErase);

public:
    ChainStrategy();
    ChainStrategy(bool bulkSearchFlag);

    string getName() override;
    Action getAction(Game& game) override;

    int getQueueSize();
    void clearQueue();
};

#endif //CODEVS_REBORN_CHAINSTRATEGY_H
