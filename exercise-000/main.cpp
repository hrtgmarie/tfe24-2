#include <fmt/chrono.h>
#include <fmt/format.h>

#include "config.h"

void changeValueByParameter(int value)
{
    value = 99;
    fmt::print("Value inside function: {}\n", value);
};

void changeValueByPointer(int * ptr) 
{
    *ptr = 99;
    fmt::print("Value inside function: {}\n", *ptr);
};

auto main(int argc, char **argv) -> int
{

    /**
     * The {fmt} lib is a cross platform library for printing and formatting text
     * it is much more convenient than std::cout and printf
     * More info at https://fmt.dev/latest/api.html
     */
    fmt::print("Hello, {}!\n", tfe24::PROJECT_NAME);

    int x = 10; 
    int * p = &x;
    fmt::print("value of x {}!\n", x);
    fmt::print("address of x {}!\n",fmt::ptr(&x));
    fmt::print("inhalt von p {}!\n", fmt::ptr(p));
    fmt::print("Wert auf den p zeigt {}!\n", *p);
    *p = 42;
    fmt::print("value of x {}!\n", x);
    fmt::print("Wert auf den p zeigt  {}!\n", *p);

    double* doublePointer = new double(3.1415);
    fmt::print("value of doublePointer: {}\n", *doublePointer);
    fmt::print("address in doublePointer: {}\n", fmt::ptr(doublePointer));
    delete doublePointer;

    int value = 10;
    fmt::print("Value before function call: {}\n", value);
    changeValueByParameter(value);
    fmt::print("Value after function call: {}\n", value);

    int value2 = 10;
    int * ptrToValue2 = &value2;
    fmt::print("Value before function call: {}\n", value2);
    changeValueByPointer(ptrToValue2);
    fmt::print("Value after function call: {}\n", value2);


    delete pD;
    return 0; /* exit gracefully*/
}