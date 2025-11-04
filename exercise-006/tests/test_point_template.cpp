#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <type_traits>
#include <fmt/core.h>

#include "../point.hpp"

TEST_CASE("Point<T>: Konstruktion & move (int)") {
    Point<int> p{2, 3};
    REQUIRE(p.x == 2);
    REQUIRE(p.y == 3);

    p.move(1, -4);
    REQUIRE(p.x == 3);
    REQUIRE(p.y == -1);
}

TEST_CASE("Point<T>: Konstruktion & move (double)") {
    Point<double> p{2.5, -3.75};
    p.move(0.5, 0.25);
    REQUIRE(p.x == Catch::Approx(3.0));
    REQUIRE(p.y == Catch::Approx(-3.5));
}

TEST_CASE("Point<T>: distance_to – Typ & Wert") {
    Point<int> a{0,0}, b{3,4};
    using dist_t = decltype(a.distance_to(b));
    STATIC_REQUIRE(!std::is_same<dist_t, int>::value); // Rückgabetyp darf kein int sein

    REQUIRE(a.distance_to(b) == Catch::Approx(5.0).margin(1e-12));
    REQUIRE(b.distance_to(a) == Catch::Approx(5.0).margin(1e-12));
    REQUIRE(a.distance_to(a) == Catch::Approx(0.0).margin(1e-12));
}

TEST_CASE("Point<T>: Operatoren (+ und Skalarmultiplikation)") {
    Point<int> a{1,2};
    Point<int> b{3,4};
    auto c = a + b;
    REQUIRE(c.x == 4);
    REQUIRE(c.y == 6);

    auto scaled = a * 2; // ergibt Point<int>
    REQUIRE(scaled.x == 2);
    REQUIRE(scaled.y == 4);
}

TEST_CASE("Point<T>: fmt-Formatter") {
    Point<int> pi{1,2};
    REQUIRE(fmt::format("{}", pi) == "(1, 2)");
}