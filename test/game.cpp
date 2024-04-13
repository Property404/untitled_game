#include "../Game.hpp"
#include "harness.hpp"

TEST_CASE(construct_game, {
        Game game(200,200);

        game.step();
});
