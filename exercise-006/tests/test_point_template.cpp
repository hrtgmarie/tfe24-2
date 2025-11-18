#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <type_traits>
#include <fmt/core.h>

#include "../point.hpp"

/*
  Unit tests for Point<T>
  -----------------------
  Tests focus on:
  - Constructors and default initialization
  - move() accumulation semantics
  - distance_to numerical correctness (use Catch::Approx for floating checks)
  - operator+ and operator* behavior and result types
  - fmt formatting output string
*/

TEST_CASE("Point<T>: Construction & move (int)") {
    Point<int> p{2, 3};
    REQUIRE(p.x == 2);
    REQUIRE(p.y == 3);

    p.move(1, -4); // in-place add: expected x=3, y=-1
    REQUIRE(p.x == 3);
    REQUIRE(p.y == -1);
}

TEST_CASE("Point<T>: Construction & move (double)") {
    Point<double> p{2.5, -3.75};
    p.move(0.5, 0.25);
    // Use Approx for floating-point comparisons because of potential rounding
    REQUIRE(p.x == Catch::Approx(3.0));
    REQUIRE(p.y == Catch::Approx(-3.5));
}

TEST_CASE("Point<T>: distance_to – type and value") {
    Point<int> a{0,0}, b{3,4};
    using dist_t = decltype(a.distance_to(b));
    // Ensure the return type is not an integer (we expect a floating type)
    STATIC_REQUIRE(!std::is_same<dist_t, int>::value);

    REQUIRE(a.distance_to(b) == Catch::Approx(5.0).margin(1e-12));
    REQUIRE(b.distance_to(a) == Catch::Approx(5.0).margin(1e-12));
    REQUIRE(a.distance_to(a) == Catch::Approx(0.0).margin(1e-12));
}

TEST_CASE("Point<T>: operators (+ and scalar multiplication)") {
    Point<int> a{1,2};
    Point<int> b{3,4};
    auto c = a + b;
    REQUIRE(c.x == 4);
    REQUIRE(c.y == 6);

    // Test scalar multiplication via member-template operator*
    auto scaled = a * 2;
    REQUIRE(scaled.x == 2);
    REQUIRE(scaled.y == 4);
}

TEST_CASE("Point<T>: fmt formatter produces expected textual representation") {
    Point<int> pi{1,2};
    REQUIRE(fmt::format("{}", pi) == "(1, 2)");
}