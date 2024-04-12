#include "../Board.hpp"
#include "harness.hpp"

TEST_CASE(clear_screen, {
        Board board(100,100);
        board.clear(Color(255,0,0));
        REQUIRE_EQUALS(board.pixels().at(42), Color(255,0,0));
        board.clear(Color(0,255,0));
        REQUIRE_EQUALS(board.pixels().at(42), Color(0,255,0));
});
