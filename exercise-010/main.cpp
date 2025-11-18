#include <fmt/chrono.h>
#include <fmt/format.h>
#include <chrono>

#include "CLI/CLI.hpp"
#include "config.h"
#include "myvector.hpp"

auto main(int argc, char** argv) -> int {
    // CLI11 command line parser setup
    int test_count = 100000;
    bool verbose = false;
    
    CLI::App app{PROJECT_NAME};
    app.add_option("-n,--count", test_count, "Number of push_back operations")
        ->default_val(100000);
    app.add_flag("-v,--verbose", verbose, "Enable verbose output");

    try {
        app.set_version_flag("-V,--version", 
            fmt::format("{} {}", PROJECT_VER, PROJECT_BUILD_DATE));
        app.parse(argc, argv);
    } catch (const CLI::ParseError& e) {
        return app.exit(e);
    }

    fmt::print("Benchmark: {}\n\n", app.get_name());
    fmt::println("========== tfe24::myvector Benchmark Suite ==========\n");

    // Test 1: Basic operations
    fmt::println("Test 1: Basic Operations");
    tfe24::myvector<int> v;
    fmt::println("  Create: tfe24::myvector<int> v;");
    fmt::println("  -> size = {}, capacity = {}", v.size(), v.capacity());
    
    v.push_back(10);
    v.push_back(20);
    v.push_back(30);
    fmt::println("  After 3x push_back: size = {}, capacity = {}", v.size(), v.capacity());
    fmt::println("  Elements: {} {} {}\n", v[0], v[1], v[2]);

    // Test 2: Copy semantics
    fmt::println("Test 2: Copy Semantics (Rule of Three)");
    fmt::println("  Create: tfe24::myvector<int> v2 = v;");
    tfe24::myvector<int> v2 = v;
    fmt::println("  v2 size = {}, v2[0] = {}", v2.size(), v2[0]);
    fmt::println("  Modify v[0] = 999");
    v[0] = 999;
    fmt::println("  v[0] = {}, v2[0] = {} (independent!)\n", v[0], v2[0]);

    // Test 3: Bounds checking
    fmt::println("Test 3: Bounds Checking with at()");
    try {
        fmt::println("  Attempt: v.at(100) with size = {}", v.size());
        v.at(100);
    } catch (const std::out_of_range& e) {
        fmt::println("  -> Exception caught: {}\n", e.what());
    }

    // Test 4: Resize and clear
    fmt::println("Test 4: Resize and Clear");
    fmt::println("  Current: size = {}, capacity = {}", v.size(), v.capacity());
    v.resize(10);
    fmt::println("  After resize(10): size = {}, capacity = {}", v.size(), v.capacity());
    v.clear();
    fmt::println("  After clear(): size = {}, capacity = {} (memory retained!)\n", 
                 v.size(), v.capacity());

    // Test 5: Reserve pre-allocation
    fmt::println("Test 5: Reserve Pre-allocation");
    tfe24::myvector<int> v3;
    fmt::println("  Create empty vector");
    fmt::println("  Call reserve(50)");
    v3.reserve(50);
    fmt::println("  -> capacity = {} (but size = {})\n", v3.capacity(), v3.size());

    // Test 6: Performance without reserve
    fmt::println("Test 6: Performance - Without Reserve");
    fmt::println("  Running {} push_back operations...", test_count);
    
    tfe24::myvector<int> perf_no_reserve;
    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < test_count; ++i) {
        perf_no_reserve.push_back(i);
    }
    auto end = std::chrono::steady_clock::now();
    auto duration_no_reserve = 
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    fmt::println("  Time: {} ms", duration_no_reserve.count());
    fmt::println("  Final: size = {}, capacity = {}\n", 
                 perf_no_reserve.size(), perf_no_reserve.capacity());

    // Test 7: Performance with reserve
    fmt::println("Test 7: Performance - With Reserve({}) Pre-allocated", test_count);
    fmt::println("  Running {} push_back operations...", test_count);
    
    tfe24::myvector<int> perf_with_reserve;
    perf_with_reserve.reserve(test_count);
    
    start = std::chrono::steady_clock::now();
    for (int i = 0; i < test_count; ++i) {
        perf_with_reserve.push_back(i);
    }
    end = std::chrono::steady_clock::now();
    auto duration_with_reserve = 
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    fmt::println("  Time: {} ms", duration_with_reserve.count());
    fmt::println("  Final: size = {}, capacity = {}\n", 
                 perf_with_reserve.size(), perf_with_reserve.capacity());

    // Test 8: Comparison
    fmt::println("Test 8: Performance Comparison");
    fmt::println("  Without reserve: {} ms", duration_no_reserve.count());
    fmt::println("  With reserve:    {} ms", duration_with_reserve.count());
    
    if (duration_with_reserve.count() > 0) {
        double speedup = static_cast<double>(duration_no_reserve.count()) / 
                        static_cast<double>(duration_with_reserve.count());
        fmt::println("  Speedup: {:.2f}x faster with reserve!\n", speedup);
    }

    if (verbose) {
        fmt::println("Detailed Analysis:");
        fmt::println("  - Growth strategy: Doubling on demand");
        fmt::println("  - Without reserve: Allocations for 1->1->2->4->8->16->...");
        fmt::println("  - With reserve: Only one allocation upfront");
        fmt::println("  - Therefore significantly faster!");
        fmt::println("  - Parameters:");
        fmt::println("    * -n, --count: {} (push_back operations)", test_count);
        fmt::println("    * -v, --verbose: {}\n", verbose);
    }

    fmt::println("========== Benchmark Complete ==========\n");

    return 0;
}
