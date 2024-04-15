#include "../Clamped.hpp"
#include "harness.hpp"

TEST_CASE(unsigned_integer, {
    Clamped<unsigned> a(0, 0, 100);
    REQUIRE_EQUALS(a, 0);
    REQUIRE_EQUALS(a + 1, 1);
    REQUIRE_EQUALS(a - 1, 0);
})

TEST_CASE(signed_integer, {
    Clamped a(0, 0, 100);

    REQUIRE_EQUALS(a, 0);
    REQUIRE_EQUALS(a + 5, 5);
    REQUIRE_EQUALS(a + 100, 100);
    REQUIRE_EQUALS(a + 1050, 100);
    REQUIRE_EQUALS(a - 1, 0);
    REQUIRE_EQUALS(a - 100, 0);
    REQUIRE_EQUALS((a + 5) * 100, 100);
    REQUIRE_EQUALS((a % 5 + 1) * 100, 100);

    a += 1;
    REQUIRE_EQUALS(a, 1);
    REQUIRE_EQUALS(a * 50, 50);
    REQUIRE_EQUALS(a * 500, 100);

    a *= 1000;
    REQUIRE_EQUALS(a, 100);

    a /= 2;
    REQUIRE_EQUALS(a, 50);

    a = 40;
    REQUIRE_EQUALS(a, 40);

    a = 400;
    REQUIRE_EQUALS(a, 100);

    // postfix
    a = 50;
    REQUIRE_EQUALS(a++, 50);
    REQUIRE_EQUALS(a--, 51);
    REQUIRE_EQUALS(a, 50);
})
