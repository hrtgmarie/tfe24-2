#include <fmt/format.h>
#include <fmt/chrono.h>

#include "CLI/CLI.hpp"
#include "config.h"

#include "point.hpp"

auto main(int argc, char **argv) -> int
{
    CLI::App app{PROJECT_NAME};
    try
    {
        app.set_version_flag("-V,--version", fmt::format("{} {}", PROJECT_VER, PROJECT_BUILD_DATE));
        app.parse(argc, argv);
    }
    catch (const CLI::ParseError &e)
    {
        return app.exit(e);
    }

    fmt::print("Hello, {}!\n", app.get_name());

    // Beispiel mit int
    Point<int> pi{2, 3};
    fmt::print("pi initial: {}\n", pi);

    // mehrere moves summieren sich
    pi.move(1, -1);
    pi.move(2, 4);
    fmt::print("pi nach zwei moves (sollte summiert sein): {}\n", pi);

    // Operatoren: + und * (Skalierung)
    Point<int> add = pi + Point<int>{1, 1};
    fmt::print("pi + (1,1) = {}\n", add);
    auto scaled = pi * 2; // ergibt Point<common_type_t<int,int>> == Point<int>
    fmt::print("pi * 2 = {}\n", scaled);

    // Vergleich
    Point<int> p_equal{5, 6};
    fmt::print("pi == p_equal ? {}\n", (pi == p_equal) ? "true" : "false");

    // Beispiel mit double
    Point<double> pd{2.5, -3.75};
    fmt::print("pd initial: {}\n", pd);

    pd.move(0.5, 0.25);
    fmt::print("pd nach move: {}\n", pd);

    // mehrere moves aufsummiert
    pd.move(1.0, 1.0);
    pd.move(-0.5, 0.5);
    fmt::print("pd nach zwei weiteren moves: {}\n", pd);

    // Abstand (dist_t ist common_type mit double)
    auto dist = pd.distance_to(Point<double>{0.0, 0.0});
    fmt::print("Abstand pd -> Ursprung = {:.6f}\n", dist);

    return 0;
}
