#include <fmt/format.h>
#include <fmt/chrono.h>

#include "CLI/CLI.hpp"
#include "config.h"

#include "point.hpp"

/*
  Main program
  ------------
  - Demonstrates usage of Point<int> and Point<double>.
  - Shows move accumulation behavior and operator usage.
  - Uses fmt::print to output human-readable representations via the
    fmt::formatter<Point<T>> provided in the header.
  - CLI11 is kept for command-line flags (version flag) per project convention.
*/

auto main(int argc, char **argv) -> int
{
    CLI::App app{PROJECT_NAME};
    try
    {
        // Register version flag using config constants (cmake-generated)
        app.set_version_flag("-V,--version", fmt::format("{} {}", PROJECT_VER, PROJECT_BUILD_DATE));
        app.parse(argc, argv);
    }
    catch (const CLI::ParseError &e)
    {
        // Let CLI11 print helpful usage info and exit with appropriate code.
        return app.exit(e);
    }

    fmt::print("Hello, {}!\n", app.get_name());

    // Example with integer coordinates
    Point<int> pi{2, 3};
    fmt::print("pi initial: {}\n", pi);

    // Multiple moves accumulate: move is in-place and additive.
    pi.move(1, -1);
    pi.move(2, 4);
    fmt::print("pi after two moves (should be summed): {}\n", pi);

    // Vector addition returns a new Point
    Point<int> add = pi + Point<int>{1, 1};
    fmt::print("pi + (1,1) = {}\n", add);

    // Scalar multiplication example uses member-template operator*
    // -> result type follows std::common_type rules.
    auto scaled = pi * 2; // Point<int> * int -> Point<int>
    fmt::print("pi * 2 = {}\n", scaled);

    // Equality check prints a human readable boolean
    Point<int> p_equal{5, 6};
    fmt::print("pi == p_equal ? {}\n", (pi == p_equal) ? "true" : "false");

    // Example with floating-point coordinates
    Point<double> pd{2.5, -3.75};
    fmt::print("pd initial: {}\n", pd);

    pd.move(0.5, 0.25);
    fmt::print("pd after move: {}\n", pd);

    pd.move(1.0, 1.0);
    pd.move(-0.5, 0.5);
    fmt::print("pd after two more moves: {}\n", pd);

    // Distance uses dist_t = common_type<T,double>, yielding a floating result.
    auto dist = pd.distance_to(Point<double>{0.0, 0.0});
    fmt::print("Distance pd -> origin = {:.6f}\n", dist);

    return 0;
}
