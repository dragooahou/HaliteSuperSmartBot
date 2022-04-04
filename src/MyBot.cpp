#include <random>
#include <ctime>

#include "hlt/game.hpp"
#include "hlt/constants.hpp"
#include "hlt/log.hpp"
#include "MainBehaviour.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

using namespace std;
using namespace hlt;

int main(int argc, char* argv[]) {

    // Rng
    unsigned int rng_seed;
    if (argc > 1) {
        rng_seed = static_cast<unsigned int>(stoul(argv[1]));
    } else {
        rng_seed = static_cast<unsigned int>(time(nullptr));
    }
    mt19937 rng(rng_seed);

    // Attach process time
//    Sleep(5000);

    Game game;

    MainBehaviour mainBehaviour(&game);

    game.ready("Halite Super Smart Bot");

    log::log("Successfully created bot! My Player ID is " + to_string(game.my_id) + ". Bot rng seed is " + to_string(rng_seed) + ".");

    for (;;) {
        game.update_frame();

        vector<Command> command_queue = mainBehaviour.Evaluate();

        if (!game.end_turn(command_queue)) {
            break;
        }
    }

    return 0;
}
