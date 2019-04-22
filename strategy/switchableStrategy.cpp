#include "randomStrategy.h"
#include "onlyChainStrategy.h"
#include "prioritizeSkillStrategy.h"
#include "antiSkillStrategy.h"
#include "switchableStrategy.h"

SwitchableStrategy::SwitchableStrategy() : game(nullptr) {}

string SwitchableStrategy::getName() {
    return "iwashiAI_v10.17";
}

Action SwitchableStrategy::getAction(Game &game) {
    this->game = &game;
    gameHistory.push_back(game);

    cerr << "--- turn " << game.turn << " ---" << endl;

    cerr << " ac:" << game.player[0].field.countAvailableCell() << "-" << game.player[1].field.countAvailableCell() << endl;

    if (game.turn > 0
        && gameHistory[game.turn - 1].player[0].field.countNumberBlock() - game.player[0].field.countNumberBlock() >= 20) {
        isChained = true;
    }

    if (!isChainedEnemy && game.turn > 0
        && gameHistory[game.turn - 1].player[1].field.countNumberBlock() - game.player[1].field.countNumberBlock() >= 7) {
        cerr << " enemy chained!" << endl;
        isChainedEnemy = true;
    }

    if (isChained && !isChainedEnemy && skillPrioritizeCheck()) {
        onlyChainStrategy.clearQueue();
        return antiSkillStrategy.getAction(game);
    }

    return onlyChainStrategy.getAction(game);
}

bool SwitchableStrategy::skillPrioritizeCheck() {
    int chainCnt = 0;
    for (int i = game->turn - 1; i >= 0; i--) {
        int blocks1 = gameHistory[i + 1].player[1].field.countNumberBlock();
        int blocks2 = gameHistory[i].player[1].field.countNumberBlock();
        if (blocks1 < blocks2 + 3) chainCnt++;
    }
    return chainCnt >= game->turn * 0.4;
}
