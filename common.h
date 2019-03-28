#ifndef CODEVS_REBORN_COMMON_H
#define CODEVS_REBORN_COMMON_H

#include <iostream>
#include <limits>
#include <string>
#include <array>

using namespace std;

#define rep(i, n) for (int i = 0; i < (n); i++)
#define itrep(i, a) for (auto i = (a).begin(); i != (a).end(); i++)
#define all(a) (a).begin(), (a).end()
#define mp(a, b) make_pair((a), (b))

const int PACK_SIZE = 2;
const int FIELD_HEIGHT = 16;
const int FIELD_WIDTH = 10;
const int MAX_TURN_NUM = 500;
const int SKILL_GAGE_THRESHOLD = 80;

using Pack = array<array<int, PACK_SIZE>, PACK_SIZE>;
using Field = array<array<int, FIELD_WIDTH>, FIELD_HEIGHT>;

class Player {
public:
    int leftTime;
    int obstacleStock;
    int skillGage;
    Field field;

    void input(istream& is) {
        is >> leftTime;
        is >> obstacleStock;
        is >> skillGage;
        rep(y, FIELD_HEIGHT) rep(x, FIELD_WIDTH) is >> field[y][x];
        string end;
        is >> end;
    }
};

class Game {
public:
    int turn;
    Pack packs[MAX_TURN_NUM];
    Player player[2];

    void inputPackInfo(istream& is) {
        string end;
        for (auto pack : packs) {
            rep(y, PACK_SIZE) rep(x, PACK_SIZE) is >> pack[y][x];
            is >> end;
        }
    }

    void inputTurnInfo(istream& is) {
        is >> turn;
        player[0].input(is);
        player[1].input(is);
    }
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

    static Action createDropPackAction(int position, int rotation) {
        Action action{};
        action.type = ActionType::DROP_PACK;
        action.position = position;
        action.rotation = rotation;
        return action;
    }

    static Action createExplodeAction() {
        Action action{};
        action.type = ActionType::EXPLODE;
        return action;
    }
};

class IStrategy {
protected:
    unsigned long randXor() {
        static unsigned long x = 123456789, y = 362436069, z = 521288629, w = 88675123;
        unsigned long t = (x ^ (x << 11));
        x = y;
        y = z;
        z = w;
        return (w = (w ^ (w >> 19)) ^ (t ^ (t >> 8)));
    }

public:
    virtual string getName() = 0;
    virtual Action getAction(Game& game) = 0;
};

#endif //CODEVS_REBORN_COMMON_H
