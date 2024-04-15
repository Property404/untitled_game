#include "../Game.hpp"
#include "harness.hpp"
#include <cstdlib>
#include <ctime>

TEST_CASE(construct_game, {
    Game game(200, 200);

    game.step();

    std::srand(std::time(nullptr));
    for (int i = 0; i < 10000; i++) {
        const auto direction = rand() % 100;
        if (direction == 0) {
            game.onKeyDown(KeyPress::Right);
        } else if (direction == 1) {
            game.onKeyDown(KeyPress::Left);
        } else if (direction == 2) {
            game.onKeyDown(KeyPress::Up);
        } else if (direction == 3) {
            game.onKeyDown(KeyPress::Down);
        }
        game.step();
    }
})
