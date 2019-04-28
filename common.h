#ifndef CODEVS_REBORN_COMMON_H
#define CODEVS_REBORN_COMMON_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <string>
#include <cstring>
#include <array>
#include <vector>
#include <queue>
#include <algorithm>
#include <cassert>
#include <set>
#include <unordered_set>
#include <random>
#include <ctime>
#include <memory>

using namespace std;

#define rep(i, n) for (int i = 0; i < (n); i++)
#define REP(i, n, m) for (int i = (n); i < (m); i++)
#define itrep(i, a) for (auto i = (a).begin(); i != (a).end(); i++)
#define all(a) (a).begin(), (a).end()
#define mp(a, b) make_pair((a), (b))

const int PACK_SIZE = 2;                // パックの大きさ
const int FIELD_HEIGHT = 16;            // フィールドの高さ
const int FIELD_WIDTH = 10;             // フィールドの幅
const int MAX_TURN_NUM = 500;           // 最大ターン数
const int EXPLODE_NUM = 5;              // 爆発するブロックの数字
const int ERASE_SUM = 10;               // 消滅する和の数
const int OBSTACLE = 11;                // お邪魔ブロックを示す数字
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
using RowField = array<array<int, FIELD_WIDTH>, FIELD_HEIGHT + PACK_SIZE + 1>;

class Logger {
public:
    static bool writeFile;
    static string fileName;

    static void init();

    void print(long long num);
    void print(string str);
    void print(const char* chr);

    void printLine(long long num);
    void printLine(string str);
    void printLine(const char* chr);
    void printLine();
};

class Point {
public:
    int x{}, y{};

    Point();
    Point(int x, int y);
};

struct ChainInfo {
    int chainNum;
    int eraseBlockNum;
    int robustNum;
    Point erasePoint;

    ChainInfo() : chainNum(0), eraseBlockNum(0), robustNum(-2), erasePoint(-1, -1) {};
};

class Field {
private:
    static long long hashSeed[FIELD_HEIGHT + PACK_SIZE + 1][FIELD_WIDTH][OBSTACLE + 1];

    RowField field{};
    bool columnUpdated[FIELD_WIDTH]{};  // 自由落下時、このフラグが立っている列のみ計算する

    // ブロックを自由落下させる
    void freeFall();

    // 消えるブロックを消す
    // 消えたブロックの数を返す
    int eraseBlocks();

    // ブロックがデンジャーゾーンに達していないかを調べる
    bool isAlive();

    ChainInfo dropWithInfo(int position);

    int columnHeight[FIELD_WIDTH];
    int packPosition;

public:
    Field();

    void input(istream &is);

    // パックを指定の位置に落とす
    // 返り値は連鎖数。ただし、デンジャーゾーンを超えた場合 -1 が返る
    int dropPack(const Pack& pack, int position, int rotation);
    int drop();
    ChainInfo dropPackWithInfo(const Pack& pack, int position, int rotation);

    void fallObstacle();

    // 点 (x, y) がフィールド内の点か否かを返す
    bool inField(int x, int y);

    // field[y][x] の値を block に上書き
    void update(int x, int y, int block);

    // 1~9 のブロック数を返す
    int countNumberBlock();
    int countBlock();
    int countExplodeBlockNum();

    // ブロックで埋められていないセルの数を返す
    int countAvailableCell();

    // 一番高いところにあるブロックの y 座標を返す
    int getMaxBlockHeight();

    // 指定したセルの埋まりにくさを返す
    int calcRobustNum(int x, int y);

    static void initHashSeed();
    unsigned long long getHash();

    void print(ostream& os);

    array<int, FIELD_WIDTH>& operator[](int idx);
};

class Player {
public:
    int leftTime{};
    int obstacleStock{};
    int skillGage{};
    long long score{};
    Field field;

    void input(istream& is);
    void fallObstacles();
    void increaseSkillGage();
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

class Timer {
    unsigned long long beginCycle;

    unsigned long long getCycle() {
        unsigned int low, high;
        __asm__ volatile ("rdtsc" : "=a" (low), "=d" (high));
        return ((unsigned long long int)low) | ((unsigned long long int)high << 32);
    }

public:
    Timer();

    double getTime();
};

class RandomNumberGenerator {
public:
    unsigned long long rand();
};

// AI の思考はこのクラスを継承し、getAction(Game&) として記述する
class IStrategy {
protected:
    RandomNumberGenerator rng;

public:
    virtual string getName() = 0;
    virtual Action getAction(Game& game) = 0;
};

#endif //CODEVS_REBORN_COMMON_H
