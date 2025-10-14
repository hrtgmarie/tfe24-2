#include <fmt/chrono.h>
#include <fmt/format.h>

#include "config.h"

void changeValueByParameter(int value)
{
    value = 99;
}

void changeValueByReference(int& value)
{
    value = 77;
}

auto main(int argc, char **argv) -> int
{

    /**
     * The {fmt} lib is a cross platform library for printing and formatting text
     * it is much more convenient than std::cout and printf
     * More info at https://fmt.dev/latest/api.html
     */
    fmt::print("Hello, {}!\n", tfe24::PROJECT_NAME);

    int x = 10; 
    int& ref_x = x;
    fmt::print("x = {}, ref_x = {}\n", x, ref_x);
    ref_x = 42;
    fmt::print("x = {}, ref_x = {}\n", x, ref_x);

    changeValueByParameter(x);
    fmt::print("x = {}, ref_x = {}\n", x, ref_x);

    changeValueByReference(ref_x);
    fmt::print("x = {}, ref_x = {}\n", x, ref_x);

    int y = 5;
    fmt::print("befor using y = {}\n", y);
    changeValueByParameter(y);
    fmt::print("changeValueByParameter y = {}\n", y);
    changeValueByReference(y);
    fmt::print("changeValueByReference y = {}\n", y);

    return 0; /* exit gracefully*/
}
