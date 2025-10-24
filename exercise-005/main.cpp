#include <fmt/chrono.h>
#include <fmt/format.h>

#include <nlohmann/json.hpp>

#include "CLI/CLI.hpp"
#include "config.h"

#include "point.hpp"

// for convenience
using json = nlohmann::json;

auto main(int argc, char **argv) -> int
{
    /**
     * CLI11 is a command line parser to add command line options
     * More info at https://github.com/CLIUtils/CLI11#usage
     */
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

    /**
     * The {fmt} lib is a cross platform library for printing and formatting text
     * it is much more convenient than std::cout and printf
     * More info at https://fmt.dev/latest/api.html
     */
    fmt::print("Hello, {}!\n", app.get_name());

    Point p1(2, 3);
    p1.print();                // Ausgabe: (2, 3)

    p1.move(1, -1);
    p1.print();                // Ausgabe: (3, 2)

    Point p2;
    p2.print();                // Ausgabe: (0, 0)

    double d = p1.distance_to(p2);
    fmt::print("Abstand p1 -> p2 = {:.4f}\n", d);

    return 0; /* exit gracefully*/
}


