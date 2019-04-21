#include "randomStrategy.h"
#include "onlyChainStrategy.h"
#include "prioritizeSkillStrategy.h"
#include "antiSkillStrategy.h"
#include "switchableStrategy.h"

SwitchableStrategy::SwitchableStrategy() : game(nullptr) {}

string SwitchableStrategy::getName() {
    return "iwashiAI_v10.12";
}

Action SwitchableStrategy::getAction(Game &game) {
    this->game = &game;
    gameHistory.push_back(game);

    if (game.turn > 0
        && gameHistory[game.turn - 1].player[0].field.countNumberBlock() - game.player[0].field.countNumberBlock() >= 20) {
        isChained = true;
    }

    if (!isChainedEnemy && game.turn > 0
        && gameHistory[game.turn - 1].player[1].field.countNumberBlock() - game.player[1].field.countNumberBlock() >= 7) {
        isChainedEnemy = true;
        cerr << "Enemy chained!!!!" << endl;
    }

    if (isChained && !isChainedEnemy && skillPrioritizeCheck()) {
        cerr << "use AntiSkill" << endl;
        onlyChainStrategy.clearQueue();
        return antiSkillStrategy.getAction(game);
    }

    cerr << "use OnlyChain" << endl;
    return onlyChainStrategy.getAction(game);
}

bool SwitchableStrategy::skillPrioritizeCheck() {
    int chainCnt = 0;
    for (int i = game->turn - 1; i >= 0; i--) {
        int blocks1 = gameHistory[i + 1].player[1].field.countNumberBlock();
        int blocks2 = gameHistory[i].player[1].field.countNumberBlock();
        if (blocks1 < blocks2 + 3) chainCnt++;
    }
    cerr << "enemyChained:" << isChainedEnemy << endl;
    return chainCnt >= game->turn * 0.4;
}
