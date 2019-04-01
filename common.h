#ifndef CODEVS_REBORN_COMMON_H
#define CODEVS_REBORN_COMMON_H

#include <iostream>
#include <limits>
#include <string>
#include <cstring>
#include <array>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

#define rep(i, n) for (int i = 0; i < (n); i++)
#define itrep(i, a) for (auto i = (a).begin(); i != (a).end(); i++)
#define all(a) (a).begin(), (a).end()
#define mp(a, b) make_pair((a), (b))

const int PACK_SIZE = 2;                // パックの大きさ
const int FIELD_HEIGHT = 16;            // フィールドの高さ
const int FIELD_WIDTH = 10;             // フィールドの幅
const int MAX_TURN_NUM = 500;           // 最大ターン数
const int ERASE_SUM = 10;               // 消滅する和の数
const int SKILL_GAGE_THRESHOLD = 80;    // スキル使用可能になるゲージのしきい値
const int INCREMENT_SKILL_GAGE = 8;     // ブロック消滅時のスキルゲージ増加量

const int DX[8] = {1, 1, 0, -1, -1, -1, 0, 1};
const int DY[8] = {0, 1, 1, 1, 0, -1, -1, -1};

enum class Direction {
    RIGHT = 0,
    TOP_RIGHT = 1,
    TOP = 2,
    TOP_LEFT = 3,
    LEFT = 4,
    BOTTOM_LEFT = 5,
    BOTTOM = 6,
    BOTTOM_RIGHT = 7
};

extern const int DECREMENT_SKILL_GAGE[];    // 敵の連鎖によるスキルゲージの減少量
extern const long long CHAIN_SCORE[];       // 通常チェインスコア
extern const long long SKILL_CHAIN_SCORE[]; // スキルチェインスコア
extern const int EXPLODE_SCORE[];           // 爆発スコア

using Pack = array<array<int, PACK_SIZE>, PACK_SIZE>;
using RowField = array<array<int, FIELD_WIDTH>, FIELD_HEIGHT + PACK_SIZE>;

class Point {
public:
    int x{}, y{};

    Point();
    Point(int x, int y);
};

class Field {
private:
    RowField field{};
    bool columnUpdated[FIELD_WIDTH]{};  // 自由落下時、このフラグが立っている列のみ計算する

    // ブロックを自由落下させ、デンジャーラインを超えなければ true, 超えれば false を返す
    bool freeFall();

    // 消えるブロックを消す
    // ブロックの消滅が発生すれば true, 発生しなければ false を返す
    bool eraseBlocks();

public:
    Field();

    // パックを指定の位置に落とす
    // 返り値は連鎖数。ただし、デンジャーゾーンを超えた場合 -1 が返る
    int dropPack(const Pack& pack, int position, int rotation);

    // 点 (x, y) がフィールド内の点か否かを返す
    bool inField(int x, int y);

    void print(ostream& os);

    array<int, FIELD_WIDTH>& operator[](int idx);
};

class Player {
public:
    int leftTime{};
    int obstacleStock{};
    int skillGage{};
    Field field;

    void input(istream& is);
};

class Game {
public:
    int turn{};
    Pack packs[MAX_TURN_NUM]{};
    Player player[2];

    void inputPackInfo(istream& is);
    void inputTurnInfo(istream& is);
};

enum class ActionType {
    DROP_PACK,
    EXPLODE
};

class Action {
public:
    ActionType type;
    int position;
    int rotation;

    static Action createDropPackAction(int position, int rotation);
    static Action createExplodeAction();
};

// AI の思考はこのクラスを継承し、getAction(Game&) として記述する
class IStrategy {
protected:
    unsigned long randXor();

public:
    virtual string getName() = 0;
    virtual Action getAction(Game& game) = 0;
};

#endif //CODEVS_REBORN_COMMON_H