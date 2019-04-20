#include "randomStrategy.h"
#include "onlyChainStrategy.h"
#include "prioritizeSkillStrategy.h"
#include "antiSkillStrategy.h"
#include "switchableStrategy.h"

SwitchableStrategy::SwitchableStrategy() : game(nullptr) {}

string SwitchableStrategy::getName() {
    return "iwashiAI_v10.9";
}

Action SwitchableStrategy::getAction(Game &game) {
    this->game = &game;
    gameHistory.push_back(game);

    if (game.turn > 0
        && gameHistory[game.turn - 1].player[0].field.countNumberBlock() - game.player[0].field.countNumberBlock() >= 20) {
        isChained = true;
    }

    if (isChained && skillPrioritizeCheck()) {
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
    cerr << "chainRate:" << (double) chainCnt / (game->turn + 1) << endl;
    return chainCnt >= game->turn * 0.4;
}
