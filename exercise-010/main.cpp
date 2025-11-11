#include <fmt/chrono.h>
#include <fmt/format.h>
#include <cassert>
#include <iostream>

#include "CLI/CLI.hpp"
#include "config.h"
#include "myvector.hpp"


auto main(int argc, char **argv) -> int
{
    /**
     * CLI11 is a command line parser to add command line options
     * More info at https://github.com/CLIUtils/CLI11#usage
     */

    // Parameter für den Performance-Test
    int N;
    bool verbose = false;

    CLI::App app{PROJECT_NAME};
    
    // CLI11 Optionen hinzufügen
    app.add_option("-n,--count", N, "Anzahl der push_back Operationen")
        ->default_val(100000);
    app.add_flag("-v,--verbose", verbose, "Verbosität einschalten");

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
    
    
    fmt::println("\n========== tfe24::myvector Test Suite ==========\n");
    
    // Test 1: Grundfunktionalität
    fmt::println("Test 1: Grundoperationen");
    tfe24::myvector<int> v;
    fmt::println("  Erstelle: tfe24::myvector<int> v;");
    fmt::println("  -> size = {}, capacity = {}", v.size(), v.capacity());
    
    v.push_back(10);
    v.push_back(20);
    v.push_back(30);
    fmt::println("  Nach 3x push_back: size = {}, capacity = {}", v.size(), v.capacity());
    fmt::println("  Elemente: {} {} {}\n", v[0], v[1], v[2]);
    
    // Test 2: Copy-Semantik
    fmt::println("Test 2: Copy-Semantik (Rule of Three)");
    fmt::println("  Erstelle: tfe24::myvector<int> v2 = v;");
    tfe24::myvector<int> v2 = v;
    fmt::println("  v2 size = {}, v2[0] = {}", v2.size(), v2[0]);
    fmt::println("  Ändere v[0] = 999");
    v[0] = 999;
    fmt::println("  v[0] = {}, v2[0] = {} (unabhängig!)\n", v[0], v2[0]);
    
    // Test 3: at() mit Grenzenprüfung
    fmt::println("Test 3: Grenzenprüfung mit at()");
    try {
        fmt::println("  Versuche: v.at(100) bei size = {}", v.size());
        v.at(100);
    } catch (const std::out_of_range& e) {
        fmt::println("  -> Exception gefangen: {}\n", e.what());
    }
    
    // Test 4: resize und clear
    fmt::println("Test 4: resize und clear");
    fmt::println("  Aktuell: size = {}, capacity = {}", v.size(), v.capacity());
    v.resize(10);
    fmt::println("  Nach resize(10): size = {}, capacity = {}", v.size(), v.capacity());
    v.clear();
    fmt::println("  Nach clear(): size = {}, capacity = {} (Speicher bleibt!)\n", 
                 v.size(), v.capacity());
    
    // Test 5: reserve
    fmt::println("Test 5: reserve() - Pre-Allocation");
    tfe24::myvector<int> v3;
    fmt::println("  Erstelle leeren Vector");
    fmt::println("  reserve(50) Aufrufen");
    v3.reserve(50);
    fmt::println("  -> capacity = {} (aber size = {})\n", v3.capacity(), v3.size());
    
    // Test 6: Performance-Messung ohne reserve
    fmt::println("Test 6: Performance - ohne Reserve");
    fmt::println("  Führe {} push_back Operationen durch...", N);
    
    tfe24::myvector<int> perf_vec;
    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < N; ++i) {
        perf_vec.push_back(i);
    }
    auto end = std::chrono::steady_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    fmt::println("  Zeit: {} ms", duration1.count());
    fmt::println("  Final: size = {}, capacity = {}\n", perf_vec.size(), perf_vec.capacity());
    
    // Test 7: Performance-Messung mit reserve
    fmt::println("Test 7: Performance - mit Reserve({}) voraus", N);
    fmt::println("  Führe {} push_back Operationen durch...", N);
    
    tfe24::myvector<int> perf_vec2;
    perf_vec2.reserve(N);  // Speicher schon reserviert!
    
    start = std::chrono::steady_clock::now();
    for (int i = 0; i < N; ++i) {
        perf_vec2.push_back(i);
    }
    end = std::chrono::steady_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    fmt::println("  Zeit: {} ms", duration2.count());
    fmt::println("  Final: size = {}, capacity = {}\n", perf_vec2.size(), perf_vec2.capacity());
    
    // Test 8: Vergleich
    fmt::println("Test 8: Vergleich");
    fmt::println("  Ohne reserve: {} ms", duration1.count());
    fmt::println("  Mit reserve:  {} ms", duration2.count());
    fmt::println("  Speedup: {:.2f}x schneller mit reserve!\n", 
                 static_cast<double>(duration1.count()) / duration2.count());
    
    if (verbose) {
        fmt::println("Detailierte Analyse:");
        fmt::println("  - Wachstumsstrategie: Verdopplung bei Bedarf");
        fmt::println("  - Ohne reserve benötigt Allokationen für: 1->1->2->4->8->...");
        fmt::println("  - Mit reserve ist nur eine Allokation nötig");
        fmt::println("  - Deshalb deutlich schneller!");
    }
    
    fmt::println("\n========== Alle Tests erfolgreich! ==========\n");
    


    return 0; /* exit gracefully*/
}
