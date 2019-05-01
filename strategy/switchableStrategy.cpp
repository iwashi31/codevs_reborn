#include "randomStrategy.h"
#include "chainStrategy.h"
#include "skillStrategy.h"
#include "antiSkillStrategy.h"
#include "switchableStrategy.h"

SwitchableStrategy::SwitchableStrategy() : game(nullptr) {}

SwitchableStrategy::State::State() = default;
SwitchableStrategy::State::State(Player& player, int score) : player(player), score(score) {}
bool SwitchableStrategy::State::operator<(const SwitchableStrategy::State &a) const {
    return score < a.score;
}

string SwitchableStrategy::getName() {
    return "iwashiAI_v10.31";
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

    if (!actionQueue.empty()) {
        onlyChainStrategy.clearQueue();

        logger.print(" q:");
        rep(i, actionQueue.size()) logger.print("*");
        logger.printLine();

        Action action = actionQueue.front();
        actionQueue.pop();
        return action;
    }

    if (isChained && !isChainedEnemy && skillPrioritizeCheck()) {
        int needTurn = (SKILL_GAGE_THRESHOLD - game.player[1].skillGage + INCREMENT_SKILL_GAGE - 1) / INCREMENT_SKILL_GAGE;
        if (onlyChainStrategy.getQueueSize() > 0 && needTurn <= onlyChainStrategy.getQueueSize()) {
            logger.printLine(" bull chain!!");
        } else {
            onlyChainStrategy.clearQueue();
            return antiSkillStrategy.getAction(game);
        }
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

        vector<State> bestStates(depth);
        logger.print(" fchain(" + name + "):");
        rep(i, depth) {
            for (auto &state : states[i + 1]) {
                if (bestStates[i].chains.empty() || bestStates[i].chains.back() < state.chains.back()) {
                    bestStates[i] = state;
                }
            }
            logger.print(bestStates[i].chains.empty() ? -1 : bestStates[i].chains.back());
            logger.print("_");
        }
        logger.printLine();
        return bestStates;
    };

    auto myBestStates = analyzeNearChains(me, "me");
    auto enemyBestStates = analyzeNearChains(enemy, "en");

    if (!enemyBestStates[2].chains.empty() && enemyBestStates[0].chains.back() <= 5 && !myBestStates[0].chains.empty() && CHAIN_SCORE[myBestStates[0].chains.back()] / 2 + game->player[1].obstacleStock >= 10) {
        const int depth = 3;
        vector<vector<State>> states(depth + 1);
        states[0].push_back(State(game->player[1], 0));
        rep(i, depth) {
            for (auto &state : states[i]) {
                state.player.fallObstacles();
                rep(position, FIELD_WIDTH - 1) rep(rotation, 4) {
                    auto nextState = state;
                    auto chainInfo = nextState.player.field.dropPackWithInfo(game->packs[game->turn + i], position, rotation);
                    if (chainInfo.chainNum == -1) continue;
                    if (i == 0) nextState.player.obstacleStock += CHAIN_SCORE[myBestStates[0].chains.back()] / 2;
                    nextState.actions.push_back(Action::createDropPackAction(position, rotation));
                    nextState.chains.push_back(chainInfo.chainNum);
                    states[i + 1].push_back(nextState);
                }
            }
        }
        bool checkmate = states[depth].empty();
        logger.printLine(" check");
        if (checkmate) {
            logger.printLine("  checkmate!");
            actionQueue.push(myBestStates[0].actions[0]);
        }
    }
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
