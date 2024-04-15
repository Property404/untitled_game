#include "../Object.hpp"
#include "harness.hpp"

TEST_CASE(zdepth, {
    std::shared_ptr<Sprite> sprite =
        std::make_shared<Sprite>(std::filesystem::path("assets/berry.rgba"), 48, 48);
    Object obj1(sprite, 0, 0);
    Object obj2(sprite, 0, 50);
    REQUIRE(obj1 < obj2);
})
