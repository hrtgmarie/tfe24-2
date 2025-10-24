#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <cmath>
#include "../point.hpp"

TEST_CASE("Point: Konstruktoren") {
    SECTION("Init mit Werten") {
        Point p{2, 3};
        REQUIRE(p.x_ == 2);
        REQUIRE(p.y_ == 3);
    }
    SECTION("Default-Konstruktor") {
        Point p;
        REQUIRE(p.x_ == 0);
        REQUIRE(p.y_ == 0);
    }
}

TEST_CASE("Point: move verschiebt relativ") {
    Point p{1, 1};
    p.move(2, -3);
    REQUIRE(p.x_ == 3);
    REQUIRE(p.y_ == -2);
}

TEST_CASE("Point: distance_to – euklidisch & robust") {
    Point a{0, 0};
    Point b{3, 4};
    REQUIRE( a.distance_to(b) == Catch::Approx(5.0).margin(1e-12) );
    REQUIRE( b.distance_to(a) == Catch::Approx(5.0).margin(1e-12) );
    REQUIRE( a.distance_to(a) == Catch::Approx(0.0).margin(1e-12) );
}

TEST_CASE("Default constructor yields origin", "[point]") {
    Point p;
    REQUIRE(p.distance_to(Point(0.0, 0.0)) == Catch::Approx(0.0));
}

TEST_CASE("Constructor sets coordinates correctly and distance is correct", "[point]") {
    Point a(2.0, 3.0);
    Point b(3.0, 2.0);
    double expected = std::sqrt((2.0-3.0)*(2.0-3.0) + (3.0-2.0)*(3.0-2.0)); // sqrt(2)
    REQUIRE(a.distance_to(b) == Catch::Approx(expected));
}

TEST_CASE("move accumulates shifts (multiple moves sum up)", "[point]") {
    Point p(0.0, 0.0);
    p.move(1, 2);
    p.move(3, 4);
    // after moves p should be at (4,6)
    REQUIRE(p.distance_to(Point(4.0, 6.0)) == Catch::Approx(0.0));
}

TEST_CASE("distance_to is symmetric", "[point]") {
    Point p1(5.0, 1.0);
    Point p2(-2.0, 4.5);
    REQUIRE(p1.distance_to(p2) == Catch::Approx(p2.distance_to(p1)));
}

