#include "randomStrategy.h"
#include "onlyChainStrategy.h"
#include "prioritizeSkillStrategy.h"
#include "antiSkillStrategy.h"
#include "switchableStrategy.h"

SwitchableStrategy::SwitchableStrategy() : game(nullptr) {}

SwitchableStrategy::State::State() = default;
SwitchableStrategy::State::State(Player& player, int score) : player(player), score(score) {}
bool SwitchableStrategy::State::operator<(const SwitchableStrategy::State &a) const {
    return score < a.score;
}

string SwitchableStrategy::getName() {
    return "iwashiAI_v10.22";
}

Action SwitchableStrategy::getAction(Game &game) {
    this->game = &game;
    gameHistory.push_back(game);

    logger.print("--- turn ");
    logger.print(game.turn);
    logger.printLine(" ---");

    analyze();

    if (game.turn > 0
        && gameHistory[game.turn - 1].player[0].field.countNumberBlock() - game.player[0].field.countNumberBlock() >= 20) {
        isChained = true;
    }

    if (!isChainedEnemy && game.turn > 0
        && gameHistory[game.turn - 1].player[1].field.countNumberBlock() - game.player[1].field.countNumberBlock() >= 7) {
        logger.printLine(" enemy chained!");
        isChainedEnemy = true;
    }

    if (isChained && !isChainedEnemy && skillPrioritizeCheck()) {
        onlyChainStrategy.clearQueue();
        return antiSkillStrategy.getAction(game);
    }

    return onlyChainStrategy.getAction(game);
}

void SwitchableStrategy::analyze() {
    logger.printLine("start analyzing");

    Player me = game->player[0];
    Player enemy = game->player[1];

    logger.print(" ac:");
    logger.print(me.field.countAvailableCell());
    logger.print("-");
    logger.printLine(enemy.field.countAvailableCell());

    if (game->turn > 0) {
        long long enemyScoreDiff = enemy.score - gameHistory[game->turn - 1].player[1].score;
        if (enemyScoreDiff != 0) {
            rep(i, 30) {
                if (enemyScoreDiff == CHAIN_SCORE[i]) {
                    logger.print(" enemy chain! : ");
                    logger.printLine(i);
                    break;
                }
                if (i == 29) {
                    logger.printLine(" enemy bomb?");
                }
            }
        }
    }

    auto analyzeNearChains = [&](Player player, string name) {
        const int depth = 3;
        vector<vector<State>> states(depth + 1);
        states[0].push_back(State(player, 0));
        rep(i, depth) {
            for (auto &state : states[i]) {
                state.player.fallObstacles();
                rep(position, FIELD_WIDTH - 1) rep(rotation, 4) {
                    auto nextState = state;
                    auto chainInfo = nextState.player.field.dropPackWithInfo(game->packs[game->turn + i], position, rotation);
                    if (chainInfo.chainNum == -1) continue;
                    nextState.actions.push_back(Action::createDropPackAction(position, rotation));
                    nextState.chains.push_back(chainInfo.chainNum);
                    states[i + 1].push_back(nextState);
                }
            }
        }
        logger.print(" fchain(" + name + "):");
        rep(i, depth) {
            int maxChain = -1;
            for (auto &state : states[i + 1]) {
                maxChain = max(maxChain, state.chains.back());
            }
            logger.print(maxChain);
            logger.print("_");
        }
        logger.printLine();
    };

    analyzeNearChains(me, "me");
    analyzeNearChains(enemy, "en");

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
