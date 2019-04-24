#include "common.h"
#include "strategy/randomStrategy.h"
#include "strategy/chainStrategy.h"
#include "strategy/skillStrategy.h"
#include "strategy/antiSkillStrategy.h"
#include "strategy/switchableStrategy.h"

int main(int argc, char* argv[]) {
    if (argc == 2 && string(argv[1]) == "--log") {
        Logger::init();
    }
    IStrategy* strategy = new SwitchableStrategy();

    Logger().printLine("*** " + strategy->getName() + " ***");

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