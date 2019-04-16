#include "common.h"
#include "strategy/randomStrategy.h"
#include "strategy/onlyChainStrategy.h"
#include "strategy/prioritizeSkillStrategy.h"

int main(int argc, char* argv[]) {
    IStrategy* strategy = new OnlyChainStrategy();

    Field::initHashSeed();

    cout << strategy->getName() << endl;
    cout.flush();

    Game game{};
    game.inputPackInfo(cin);

    rep(_, MAX_TURN_NUM) {
        game.inputTurnInfo(cin);

        auto action = strategy->getAction(game);
        if (action.type == ActionType::DROP_PACK) {
            cout << action.position << " " << action.rotation << endl;
        } else if (action.type == ActionType::EXPLODE) {
            cout << "S" << endl;
        }
        cout.flush();
    }

    return 0;
}