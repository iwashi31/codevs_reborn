#include "common.h"

const int DECREMENT_SKILL_GAGE[] = {
        0, // 0 chains
        0, // 1 chains
        0, // 2 chains
        18, // 3 chains
        20, // 4 chains
        22, // 5 chains
        24, // 6 chains
        26, // 7 chains
        28, // 8 chains
        30, // 9 chains
        32, // 10 chains
        34, // 11 chains
        36, // 12 chains
        38, // 13 chains
        40, // 14 chains
        42, // 15 chains
        44, // 16 chains
        46, // 17 chains
        48, // 18 chains
        50, // 19 chains
        52, // 20 chains
        54, // 21 chains
        56, // 22 chains
        58, // 23 chains
        60, // 24 chains
        62, // 25 chains
        64, // 26 chains
        66, // 27 chains
        68, // 28 chains
        70, // 29 chains
        72, // 30 chains
        74, // 31 chains
        76, // 32 chains
        78, // 33 chains
        80, // 34 chains
        82, // 35 chains
        84, // 36 chains
        86, // 37 chains
        88, // 38 chains
        90, // 39 chains
        92, // 40 chains
        94, // 41 chains
        96, // 42 chains
        98, // 43 chains
        100, // 44 chains
        102, // 45 chains
        104, // 46 chains
        106, // 47 chains
        108, // 48 chains
        110, // 49 chains
        112, // 50 chains
        114, // 51 chains
        116, // 52 chains
        118, // 53 chains
        120, // 54 chains
        122, // 55 chains
        124, // 56 chains
        126, // 57 chains
        128, // 58 chains
        130, // 59 chains
        132, // 60 chains
        134, // 61 chains
        136, // 62 chains
        138, // 63 chains
        140, // 64 chains
        142, // 65 chains
        144, // 66 chains
        146, // 67 chains
        148, // 68 chains
        150, // 69 chains
        152, // 70 chains
        154, // 71 chains
        156, // 72 chains
        158, // 73 chains
        160, // 74 chains
        162, // 75 chains
        164, // 76 chains
        166, // 77 chains
        168, // 78 chains
        170, // 79 chains
        172  // 80 chains
};

const long long CHAIN_SCORE[] = {
        0LL, // 0 chains
        1LL, // 1 chains
        2LL, // 2 chains
        4LL, // 3 chains
        6LL, // 4 chains
        9LL, // 5 chains
        13LL, // 6 chains
        19LL, // 7 chains
        27LL, // 8 chains
        37LL, // 9 chains
        50LL, // 10 chains
        67LL, // 11 chains
        90LL, // 12 chains
        120LL, // 13 chains
        159LL, // 14 chains
        210LL, // 15 chains
        276LL, // 16 chains
        362LL, // 17 chains
        474LL, // 18 chains
        620LL, // 19 chains
        810LL, // 20 chains
        1057LL, // 21 chains
        1378LL, // 22 chains
        1795LL, // 23 chains
        2337LL, // 24 chains
        3042LL, // 25 chains
        3959LL, // 26 chains
        5151LL, // 27 chains
        6701LL, // 28 chains
        8716LL, // 29 chains
        11335LL, // 30 chains
        14740LL, // 31 chains
        19167LL, // 32 chains
        24923LL, // 33 chains
        32405LL, // 34 chains
        42132LL, // 35 chains
        54778LL, // 36 chains
        71218LL, // 37 chains
        92590LL, // 38 chains
        120373LL, // 39 chains
        156491LL, // 40 chains
        203445LL, // 41 chains
        264485LL, // 42 chains
        343838LL, // 43 chains
        446997LL, // 44 chains
        581103LL, // 45 chains
        755441LL, // 46 chains
        982081LL, // 47 chains
        1276713LL, // 48 chains
        1659735LL, // 49 chains
        2157664LL, // 50 chains
        2804971LL, // 51 chains
        3646471LL, // 52 chains
        4740421LL, // 53 chains
        6162556LL, // 54 chains
        8011332LL, // 55 chains
        10414741LL, // 56 chains
        13539173LL, // 57 chains
        17600934LL, // 58 chains
        22881224LL, // 59 chains
        29745601LL, // 60 chains
        38669291LL, // 61 chains
        50270088LL, // 62 chains
        65351124LL, // 63 chains
        84956471LL, // 64 chains
        110443422LL, // 65 chains
        143576459LL, // 66 chains
        186649407LL, // 67 chains
        242644240LL, // 68 chains
        315437523LL, // 69 chains
        410068791LL, // 70 chains
        533089439LL, // 71 chains
        693016282LL, // 72 chains
        900921178LL, // 73 chains
        1171197543LL, // 74 chains
        1522556818LL, // 75 chains
        1979323876LL, // 76 chains
        2573121051LL, // 77 chains
        3345057379LL, // 78 chains
        4348574605LL, // 79 chains
        5653147000LL  // 80 chains
};

const long long SKILL_CHAIN_SCORE[] = {
        0LL, // 0 chains
        1LL, // 1 chains
        2LL, // 2 chains
        4LL, // 3 chains
        6LL, // 4 chains
        9LL, // 5 chains
        13LL, // 6 chains
        19LL, // 7 chains
        27LL, // 8 chains
        37LL, // 9 chains
        50LL, // 10 chains
        67LL, // 11 chains
        90LL, // 12 chains
        120LL, // 13 chains
        159LL, // 14 chains
        210LL, // 15 chains
        276LL, // 16 chains
        362LL, // 17 chains
        474LL, // 18 chains
        620LL, // 19 chains
        810LL, // 20 chains
        1057LL, // 21 chains
        1378LL, // 22 chains
        1795LL, // 23 chains
        2337LL, // 24 chains
        3042LL, // 25 chains
        3959LL, // 26 chains
        5151LL, // 27 chains
        6701LL, // 28 chains
        8716LL, // 29 chains
        11335LL, // 30 chains
        14740LL, // 31 chains
        19167LL, // 32 chains
        24923LL, // 33 chains
        32405LL, // 34 chains
        42132LL, // 35 chains
        54778LL, // 36 chains
        71218LL, // 37 chains
        92590LL, // 38 chains
        120373LL, // 39 chains
        156491LL, // 40 chains
        203445LL, // 41 chains
        264485LL, // 42 chains
        343838LL, // 43 chains
        446997LL, // 44 chains
        581103LL, // 45 chains
        755441LL, // 46 chains
        982081LL, // 47 chains
        1276713LL, // 48 chains
        1659735LL, // 49 chains
        2157664LL, // 50 chains
        2804971LL, // 51 chains
        3646471LL, // 52 chains
        4740421LL, // 53 chains
        6162556LL, // 54 chains
        8011332LL, // 55 chains
        10414741LL, // 56 chains
        13539173LL, // 57 chains
        17600934LL, // 58 chains
        22881224LL, // 59 chains
        29745601LL, // 60 chains
        38669291LL, // 61 chains
        50270088LL, // 62 chains
        65351124LL, // 63 chains
        84956471LL, // 64 chains
        110443422LL, // 65 chains
        143576459LL, // 66 chains
        186649407LL, // 67 chains
        242644240LL, // 68 chains
        315437523LL, // 69 chains
        410068791LL, // 70 chains
        533089439LL, // 71 chains
        693016282LL, // 72 chains
        900921178LL, // 73 chains
        1171197543LL, // 74 chains
        1522556818LL, // 75 chains
        1979323876LL, // 76 chains
        2573121051LL, // 77 chains
        3345057379LL, // 78 chains
        4348574605LL, // 79 chains
        5653147000LL  // 80 chains
};

const int EXPLODE_SCORE[] = {
        25, // 0 blocks
        26, // 1 blocks
        28, // 2 blocks
        29, // 3 blocks
        31, // 4 blocks
        33, // 5 blocks
        35, // 6 blocks
        37, // 7 blocks
        39, // 8 blocks
        42, // 9 blocks
        44, // 10 blocks
        47, // 11 blocks
        50, // 12 blocks
        52, // 13 blocks
        56, // 14 blocks
        59, // 15 blocks
        62, // 16 blocks
        66, // 17 blocks
        70, // 18 blocks
        74, // 19 blocks
        79, // 20 blocks
        84, // 21 blocks
        89, // 22 blocks
        94, // 23 blocks
        100, // 24 blocks
        105, // 25 blocks
        112, // 26 blocks
        118, // 27 blocks
        125, // 28 blocks
        133, // 29 blocks
        141, // 30 blocks
        149, // 31 blocks
        158, // 32 blocks
        168, // 33 blocks
        178, // 34 blocks
        188, // 35 blocks
        200, // 36 blocks
        211, // 37 blocks
        224, // 38 blocks
        237, // 39 blocks
        251, // 40 blocks
        266, // 41 blocks
        282, // 42 blocks
        299, // 43 blocks
        317, // 44 blocks
        336, // 45 blocks
        356, // 46 blocks
        377, // 47 blocks
        400, // 48 blocks
        423, // 49 blocks
        448, // 50 blocks
        475, // 51 blocks
        503, // 52 blocks
        533, // 53 blocks
        565, // 54 blocks
        599, // 55 blocks
        634, // 56 blocks
        672, // 57 blocks
        712, // 58 blocks
        755, // 59 blocks
        800, // 60 blocks
        847, // 61 blocks
        897, // 62 blocks
        951, // 63 blocks
        1007, // 64 blocks
        1067, // 65 blocks
        1131, // 66 blocks
        1198, // 67 blocks
        1269, // 68 blocks
        1345, // 69 blocks
        1425, // 70 blocks
        1510, // 71 blocks
        1600, // 72 blocks
        1695, // 73 blocks
        1795, // 74 blocks
        1902, // 75 blocks
        2015, // 76 blocks
        2135, // 77 blocks
        2262, // 78 blocks
        2397, // 79 blocks
        2539, // 80 blocks
        2690, // 81 blocks
        2850, // 82 blocks
        3020, // 83 blocks
        3200, // 84 blocks
        3390, // 85 blocks
        3591, // 86 blocks
        3805, // 87 blocks
        4031, // 88 blocks
        4271, // 89 blocks
        4525, // 90 blocks
        4794, // 91 blocks
        5079, // 92 blocks
        5381, // 93 blocks
        5701, // 94 blocks
        6040, // 95 blocks
        6400, // 96 blocks
        6780, // 97 blocks
        7183, // 98 blocks
        7610, // 99 blocks
        8063, // 100 blocks
        8542, // 101 blocks
        9050, // 102 blocks
        9589, // 103 blocks
        10159, // 104 blocks
        10763, // 105 blocks
        11403, // 106 blocks
        12081, // 107 blocks
        12800, // 108 blocks
        13561, // 109 blocks
        14367, // 110 blocks
        15221, // 111 blocks
        16126, // 112 blocks
        17085, // 113 blocks
        18101, // 114 blocks
        19178, // 115 blocks
        20318, // 116 blocks
        21526, // 117 blocks
        22807, // 118 blocks
        24163, // 119 blocks
        25600, // 120 blocks
        27122, // 121 blocks
        28735, // 122 blocks
        30443, // 123 blocks
        32253, // 124 blocks
        34171, // 125 blocks
        36203, // 126 blocks
        38356, // 127 blocks
        40637, // 128 blocks
        43053, // 129 blocks
        45614, // 130 blocks
        48326, // 131 blocks
        51200, // 132 blocks
        54244, // 133 blocks
        57470, // 134 blocks
        60887, // 135 blocks
        64507, // 136 blocks
        68343, // 137 blocks
        72407, // 138 blocks
        76713, // 139 blocks
        81274, // 140 blocks
        86107, // 141 blocks
        91228, // 142 blocks
        96652, // 143 blocks
        102400, // 144 blocks
        108489, // 145 blocks
        114940, // 146 blocks
        121774, // 147 blocks
        129015, // 148 blocks
        136687, // 149 blocks
        144815, // 150 blocks
        153426, // 151 blocks
        162549, // 152 blocks
        172215, // 153 blocks
        182456, // 154 blocks
        193305, // 155 blocks
        204800, // 156 blocks
        216978, // 157 blocks
        229880, // 158 blocks
        243549, // 159 blocks
        258031  // 160 blocks
};

bool Logger::writeFile = false;
string Logger::fileName{};

void Logger::init() {
    writeFile = true;
    time_t t = time(nullptr);
    char buf[64];
    strftime(buf, sizeof(buf), "logs/%Y-%m-%d_%H-%M-%S.log", localtime(&t));
    fileName = string(buf);
}

void Logger::print(long long num) {
    cerr << num;

    if (writeFile) {
        ofstream fout(fileName, ios::app);
        fout << num;
    }
}

void Logger::print(string str) {
    cerr << str;

    if (writeFile) {
        ofstream fout(fileName, ios::app);
        fout << str;
    }
}

void Logger::print(const char* chr) {
    print(move(string(chr)));
}

void Logger::printLine(long long num) {
    cerr << num << endl;

    if (writeFile) {
        ofstream fout(fileName, ios::app);
        fout << num << endl;
    }
}

void Logger::printLine(string str) {
    cerr << str << endl;

    if (writeFile) {
        ofstream fout(fileName, ios::app);
        fout << str << endl;
    }
}

void Logger::printLine(const char* chr) {
    printLine(string(chr));
}

void Logger::printLine() {
    printLine("");
}

Point::Point() = default;
Point::Point(int x, int y) : x(x), y(y) {}

Field::Field() {
    memset(columnUpdated, false, sizeof(columnUpdated));
    memset(columnHeight, 0, sizeof(columnHeight));
}

long long Field::hashSeed[FIELD_HEIGHT + PACK_SIZE + 1][FIELD_WIDTH][OBSTACLE + 1];

void Field::input(istream &is) {
    memset(columnHeight, 0, sizeof(columnHeight));
    for (int y = FIELD_HEIGHT + PACK_SIZE; y >= FIELD_HEIGHT; y--) rep(x, FIELD_WIDTH) field[y][x] = 0;
    for (int y = FIELD_HEIGHT - 1; y >= 0; y--) rep(x, FIELD_WIDTH) {
            is >> field[y][x];
            if (field[y][x] != 0 && columnHeight[x] == 0) columnHeight[x] = y + 1;
        }
}

void Field::freeFall() {
    rep(x, FIELD_WIDTH) {
        if (!columnUpdated[x]) continue;
        columnUpdated[x] = false;
        int bottom = 0;
        rep(y, columnHeight[x]) {
            if (field[y][x] != 0) {
                field[bottom][x] = field[y][x];
                if (bottom != y) field[y][x] = 0;
                bottom++;
            }
        }
        columnHeight[x] = bottom;
    }
}

int Field::eraseBlocks() {
    // TODO: 高速化の余地がありそう
    vector<Point> erasedPoints;
    rep(x, FIELD_WIDTH) rep(y, columnHeight[x]) {
        if (field[y][x] == 0 || field[y][x] == OBSTACLE) continue;
        rep(directionIdx, 4) {
            int ix = x + DX[directionIdx];
            int iy = y + DY[directionIdx];
            if (!inField(ix, iy)) continue;

            int sum = field[y][x] + field[iy][ix];
            if (sum == ERASE_SUM) {
                erasedPoints.emplace_back(x, y);
                erasedPoints.emplace_back(ix, iy);
            }
        }
    }

    int cnt = 0;
    for (auto p : erasedPoints) {
        cnt += field[p.y][p.x] != 0;
        field[p.y][p.x] = 0;
        columnUpdated[p.x] = true;
    }

    return cnt;
}

bool Field::isAlive() {
    rep(x, FIELD_WIDTH) {
        if (field[FIELD_HEIGHT][x] != 0) return false;
    }
    return true;
}

bool Field::inField(int x, int y) {
    return x >= 0 && x < field[0].size() && y >= 0 && y < field.size();
}

int Field::dropPack(const Pack &pack, int position, int rotation) {
    return dropPackWithInfo(pack, position, rotation).chainNum;
}

ChainInfo Field::dropPackWithInfo(const Pack &pack, int position, int rotation) {
    assert(position >= 0 && position < FIELD_WIDTH - 1);
    assert(columnHeight[position] + 1 < FIELD_HEIGHT + PACK_SIZE + 1);
    if (rotation == 0) {
        field[columnHeight[position]][position]               = pack[0][0];
        field[columnHeight[position] + 1][position]           = pack[1][0];
        field[columnHeight[position + 1] + 1][position + 1]   = pack[1][1];
        field[columnHeight[position + 1]][position + 1]       = pack[0][1];
    } else if (rotation == 1) {
        field[columnHeight[position]][position]               = pack[0][1];
        field[columnHeight[position] + 1][position]           = pack[0][0];
        field[columnHeight[position + 1] + 1][position + 1]   = pack[1][0];
        field[columnHeight[position + 1]][position + 1]       = pack[1][1];
    } else if (rotation == 2) {
        field[columnHeight[position]][position]               = pack[1][1];
        field[columnHeight[position] + 1][position]           = pack[0][1];
        field[columnHeight[position + 1] + 1][position + 1]   = pack[0][0];
        field[columnHeight[position + 1]][position + 1]       = pack[1][0];
    } else if (rotation == 3) {
        field[columnHeight[position]][position]               = pack[1][0];
        field[columnHeight[position] + 1][position]           = pack[1][1];
        field[columnHeight[position + 1] + 1][position + 1]   = pack[0][1];
        field[columnHeight[position + 1]][position + 1]       = pack[0][0];
    } else assert(false);

    columnHeight[position] += 2;
    columnHeight[position + 1] += 2;

    columnUpdated[position] = true;
    columnUpdated[position + 1] = true;

    return dropWithInfo(position);
}

int Field::drop() {
    return dropWithInfo(-1).chainNum;
}

ChainInfo Field::dropWithInfo(int position) {
    ChainInfo info;
    while (true) {
        int eraseNum;
        if (info.chainNum == 0) {
            Field prevField = *this;
            if (position != -1) {
                prevField[columnHeight[position] - 1][position] = 0;
                prevField[columnHeight[position] - 2][position] = 0;
                prevField[columnHeight[position + 1] - 1][position + 1] = 0;
                prevField[columnHeight[position + 1] - 2][position + 1] = 0;
                prevField.columnHeight[position] -= 2;
                prevField.columnHeight[position + 1] -= 2;
            }
            freeFall();
            eraseNum = eraseBlocks();
            info.robustNum = [&]() {
                rep(x, FIELD_WIDTH) rep(y, prevField.columnHeight[x]) {
                    if (prevField[y][x] != 0 && field[y][x] == 0 && prevField[y + 1][x] != 0) {
                        info.erasePoint = Point(x, y);
                        return prevField.calcRobustNum(x, y);
                    }
                }
                return -1;
            }();
        } else {
            freeFall();
            eraseNum = eraseBlocks();
        }
        if (eraseNum == 0) {
            if (!isAlive()) info.chainNum = -1;
            break;
        }
        info.eraseBlockNum += eraseNum;
        info.chainNum++;
    }

    return info;
}

void Field::fallObstacle() {
    rep(x, FIELD_WIDTH) {
        field[columnHeight[x]][x] = OBSTACLE;
        columnHeight[x]++;
    }
}

void Field::update(int x, int y, int block) {
    columnUpdated[x] = true;
    field[y][x] = block;
}

int Field::countNumberBlock() {
    int cnt = 0;
    rep(x, FIELD_WIDTH) rep(y, columnHeight[x]) {
        if (field[y][x] >= 1 && field[y][x] <= 9) cnt++;
    }
    return cnt;
}

int Field::countBlock() {
    int cnt = 0;
    rep(x, FIELD_WIDTH) {
        cnt += columnHeight[x];
    }
    return cnt;
}

int Field::countExplodeBlockNum() {
    int cnt = 0;
    rep(x, FIELD_WIDTH) rep(y, columnHeight[x]) {
            if (field[y][x] == 0 || field[y][x] == OBSTACLE) continue;
            cnt += [&]() {
                for (int dy = -1; dy <= 1; dy++) for (int dx = -1; dx <= 1; dx++) {
                        int nx = x + dx;
                        int ny = y + dy;
                        if (!inField(nx, ny)) continue;
                        if (field[ny][nx] == EXPLODE_NUM) return 1;
                    }
                return 0;
            }();
        }
    return cnt;
}

int Field::countAvailableCell() {
    int dx[8] = {1, 1, 0, -1, -1, -1, 0, 1};
    int dy[8] = {0, 1, 1, 1, 0, -1, -1, -1};

    bool visited[FIELD_HEIGHT][FIELD_WIDTH];
    memset(visited, false, sizeof(visited));

    queue<Point> q;
    rep(x, FIELD_WIDTH) {
        if (field[FIELD_HEIGHT - 1][x] == OBSTACLE) continue;
        q.push(Point(x, FIELD_HEIGHT - 1));
        visited[FIELD_HEIGHT - 1][x] = true;
    }

    int cnt = 0;
    while (!q.empty()) {
        auto p = q.front(); q.pop();
        cnt++;
        rep(dir, 8) {
            int nx = p.x + dx[dir];
            int ny = p.y + dy[dir];
            if (nx < 0 || nx >= FIELD_WIDTH || ny < 0 || ny >= FIELD_HEIGHT) continue;
            if (field[ny][nx] == OBSTACLE) continue;
            if (visited[ny][nx]) continue;
            visited[ny][nx] = true;
            q.push(Point(nx, ny));
        }
    }

    return cnt;
}

int Field::getMaxBlockHeight() {
    for (int y = FIELD_HEIGHT + PACK_SIZE; y >= 0; y--) rep(x, FIELD_WIDTH) {
        if (field[y][x] != 0) return y;
    }
    return -1;
}

int Field::calcRobustNum(int x, int y) {
    if (field[y][x] == 0 || field[y][x] == OBSTACLE) return -1;
    int ret = -1;
    rep(ty, FIELD_HEIGHT) {
        if ((x > 0 && field[ty][x - 1] == 0)
            || (x < FIELD_WIDTH - 1 && field[ty][x + 1] == 0)) {
            ret = y - ty + 1;
            break;
        }
    }
    return ret;
}

void Field::initHashSeed() {
    RandomNumberGenerator rng;
    rep(y, FIELD_HEIGHT + PACK_SIZE + 1) rep(x, FIELD_WIDTH) rep(i, OBSTACLE + 1) {
        hashSeed[y][x][i] = rng.rand();
    }
}

unsigned long long Field::getHash() {
    unsigned long long hash = 0;
    rep(y, FIELD_HEIGHT) rep(x, FIELD_WIDTH) {
        hash ^= hashSeed[y][x][field[y][x]];
    }
    return hash;
}

void Field::print(ostream& os) {
    for (int y = field.size() - 1; y >= 0; y--) {
        rep(x, FIELD_WIDTH) {
            os << field[y][x] << " ";
        }
        os << endl;
    }
}

array<int, FIELD_WIDTH>& Field::operator[](int idx) {
    return field[idx];
}

void Player::input(istream& is) {
    is >> leftTime;
    is >> obstacleStock;
    is >> skillGage;
    is >> score;
    field.input(is);
    string end;
    is >> end;
}

void Player::fallObstacles() {
    if (obstacleStock < FIELD_WIDTH) return;
    obstacleStock -= FIELD_WIDTH;
    field.fallObstacle();
}

void Player::increaseSkillGage() {
    skillGage += INCREMENT_SKILL_GAGE;
    skillGage = min(skillGage, 100);
}

void Game::inputPackInfo(istream& is) {
    string end;
    for (auto& pack : packs) {
        for (int y = PACK_SIZE - 1; y >= 0; y--) rep(x, PACK_SIZE) is >> pack[y][x];
        is >> end;
    }
}

void Game::inputTurnInfo(istream& is) {
    is >> turn;
    player[0].input(is);
    player[1].input(is);
}

Action Action::createDropPackAction(int position, int rotation) {
    Action action{};
    action.type = ActionType::DROP_PACK;
    action.position = position;
    action.rotation = rotation;
    return action;
}

Action Action::createExplodeAction() {
    Action action{};
    action.type = ActionType::EXPLODE;
    return action;
}

Timer::Timer() {
    beginCycle = getCycle();
}

double Timer::getTime() {
    return (double)(getCycle() - beginCycle) / 2500000000;
}

unsigned long long RandomNumberGenerator::rand() {
    static unsigned long long x = 123456789, y = 362436069, z = 521288629, w = 88675123;
    unsigned long long t = (x ^ (x << 11));
    x = y;
    y = z;
    z = w;
    return (w = (w ^ (w >> 19)) ^ (t ^ (t >> 8)));
}
