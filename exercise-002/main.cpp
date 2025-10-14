#include <fmt/chrono.h>
#include <fmt/format.h>

#include "CLI/CLI.hpp"
#include "config.h"

// Teil 1: Globale Variable (Data-Segment)
int globalVar = 1;

// Teil 4: Funktion (Code-Segment)
void foo() {
    fmt::print("Hello from foo!\n");
}

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

    // Teil 2: Lokale Variable (Stack)
    int localVar = 2;

    // Teil 3: Dynamisch allozierte Variable (Heap)
    int* heapVar = new int(3);

    // Ausgabe: Wert und Adresse der globalen Variable
    fmt::print("globalVar: Wert = {}, Adresse = {}\n", globalVar, fmt::ptr(&globalVar));

    // Ausgabe: Wert und Adresse der lokalen Variable
    fmt::print("localVar: Wert = {}, Adresse = {}\n", localVar, fmt::ptr(&localVar));

    // Ausgabe: Wert und Adresse der Heap-Variable
    fmt::print("heapVar: Wert = {}, Adresse = {}\n", *heapVar, fmt::ptr(heapVar));

    // Speicher wieder freigeben
    delete heapVar;

    // Adresse der Funktion foo
    fmt::print("Adresse von foo: {}\n", fmt::ptr(&foo));

    /*
    Fazit zu Speichersegmenten in C++:

    - Globale Variablen (wie globalVar) werden im Data-Segment abgelegt und sind während der gesamten Programmlaufzeit verfügbar.
    - Lokale Variablen (wie localVar) werden auf dem Stack gespeichert. Sie existieren nur innerhalb ihres Gültigkeitsbereichs und werden automatisch verwaltet.
    - Dynamisch allozierte Variablen (wie heapVar mit new) liegen im Heap. Sie müssen explizit mit delete freigegeben werden und erlauben flexible Speicherverwaltung.
    - Funktionen (wie foo) befinden sich im Code-Segment.

    Die Unterscheidung zwischen Stack und Heap ist wichtig, da der Stack begrenzt und schneller ist, aber nur für kurzlebige Daten geeignet. Der Heap bietet mehr Speicher und Flexibilität, benötigt aber manuelle Verwaltung.

    Referenzen und Zeiger sind essenziell, um auf Speicheradressen zuzugreifen. Zeiger sind notwendig, um mit Heap-Speicher zu arbeiten, während Referenzen oft für effiziente Funktionsübergaben genutzt werden.
    */

    return 0; /* exit gracefully*/
}
