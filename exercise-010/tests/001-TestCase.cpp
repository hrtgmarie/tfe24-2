#include <catch2/catch_all.hpp>
#include "myvector.hpp"

// ============================================================
// Test 1: Grundoperationen
// ============================================================
TEST_CASE("basic operations on tfe24::myvector<int>", "[myvector][basic]") {
  tfe24::myvector<int> v;
  REQUIRE(v.size() == 0);
  REQUIRE(v.capacity() == 0);

  SECTION("push_back grows size") {
    v.push_back(42);
    v.push_back(7);
    REQUIRE(v.size() == 2);
    REQUIRE(v[0] == 42);
    REQUIRE(v.at(1) == 7);
  }

  SECTION("resize larger fills with default values") {
    v.resize(5);
    REQUIRE(v.size() == 5);
    for (size_t i = 0; i < v.size(); ++i) {
      REQUIRE(v[i] == int{}); // erwartet 0 für int
    }
  }

  SECTION("resize smaller reduces size") {
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.resize(1);
    REQUIRE(v.size() == 1);
    REQUIRE(v[0] == 1);
  }

  SECTION("at throws on out of range") {
    v.push_back(1);
    REQUIRE_THROWS_AS(v.at(5), std::out_of_range);
  }

  SECTION("clear resets size but keeps capacity") {
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    size_t old_capacity = v.capacity();
    v.clear();
    REQUIRE(v.size() == 0);
    REQUIRE(v.capacity() == old_capacity);
  }
}

// ============================================================
// Test 2: Copy-Semantik (Rule of Three)
// ============================================================
TEST_CASE("copy semantics (Rule of Three)", "[myvector][copy]") {
  tfe24::myvector<int> a;
  for (int i = 0; i < 3; ++i) a.push_back(i + 10);
  
  SECTION("copy constructor creates independent copy") {
    tfe24::myvector<int> b = a;
    REQUIRE(b.size() == a.size());
    REQUIRE(b.size() == 3);
    
    // Änderungen an a dürfen b nicht beeinflussen
    a[0] = 99;
    REQUIRE(b[0] != 99);
    REQUIRE(b[0] == 10);
  }

  SECTION("copy assignment creates independent copy") {
    tfe24::myvector<int> c;
    c = a;
    REQUIRE(c.size() == a.size());
    REQUIRE(c.size() == 3);
    
    // Änderungen an a dürfen c nicht beeinflussen
    a[1] = 88;
    REQUIRE(c[1] != 88);
    REQUIRE(c[1] == 11);
  }

  SECTION("self-assignment is safe") {
    a = a;  // Self-assignment sollte funktionieren
    REQUIRE(a.size() == 3);
    REQUIRE(a[0] == 10);
  }
}

// ============================================================
// Test 3: Wachstumsverhalten
// ============================================================
TEST_CASE("growth behavior with push_back", "[myvector][growth]") {
  tfe24::myvector<int> v;
  
  REQUIRE(v.capacity() == 0);
  
  // Erstes Element: capacity sollte wachsen (nicht linear 1 pro push)
  v.push_back(1);
  size_t cap_after_1 = v.capacity();
  REQUIRE(cap_after_1 >= 1);

  // Weitere Elemente ohne Reallocation, wenn Capacity ausreicht
  v.push_back(2);
  REQUIRE(v.capacity() == cap_after_1);  // Keine neue Allokation
  
  // Capacity wächst geometrisch (z.B. Verdopplung oder 1.5x)
  // Das bedeutet: Es gibt einen "Jump" in der Capacity, nicht 1 pro Element
  while (v.size() < v.capacity()) {
    v.push_back(v.size() + 1);
  }
  size_t old_cap = v.capacity();
  
  v.push_back(100);  // Jetzt sollte Reallocation stattfinden
  size_t new_cap = v.capacity();
  
  // Neue Capacity sollte größer als alte sein (Verdopplung oder ähnlich)
  REQUIRE(new_cap > old_cap);
  // Für Verdopplung: new_cap sollte mindestens 2 * old_cap sein
  REQUIRE(new_cap >= old_cap * 1.5);  // Mindestens 1.5x-Faktor
}

// ============================================================
// Test 4: Konstruktor mit Größe
// ============================================================
TEST_CASE("constructor with size parameter", "[myvector][constructor]") {
  size_t initial_size = 10;
  tfe24::myvector<int> v(initial_size);
  
  REQUIRE(v.size() == initial_size);
  REQUIRE(v.capacity() >= initial_size);
  
  SECTION("elements initialized to default value") {
    for (size_t i = 0; i < v.size(); ++i) {
      REQUIRE(v[i] == int{});  // 0 für int
    }
  }
}

// ============================================================
// Test 5: Reserve Funktionalität
// ============================================================
TEST_CASE("reserve allocates space without changing size", "[myvector][reserve]") {
  tfe24::myvector<int> v;
  REQUIRE(v.size() == 0);
  REQUIRE(v.capacity() == 0);

  SECTION("reserve increases capacity") {
    v.reserve(50);
    REQUIRE(v.capacity() >= 50);
    REQUIRE(v.size() == 0);  // Size bleibt unverändert!
  }

  SECTION("reserve does not initialize elements") {
    v.push_back(10);
    v.push_back(20);
    size_t old_size = v.size();
    
    v.reserve(100);
    REQUIRE(v.size() == old_size);  // Size bleibt gleich
    REQUIRE(v[0] == 10);
    REQUIRE(v[1] == 20);
  }

  SECTION("reserve with smaller value than capacity does nothing") {
    v.reserve(100);
    size_t cap_100 = v.capacity();
    
    v.reserve(50);  // Kleinere Reservierung
    REQUIRE(v.capacity() == cap_100);  // Capacity ändert sich nicht
  }

  SECTION("reserve enables efficient batch insertion") {
    tfe24::myvector<int> v_reserved;
    tfe24::myvector<int> v_unreserved;
    
    // Beide mit reserve bzw. ohne
    int N = 1000;
    v_reserved.reserve(N);
    
    for (int i = 0; i < N; ++i) {
      v_reserved.push_back(i);
      v_unreserved.push_back(i);
    }
    
    // Finale Größe sollte gleich sein
    REQUIRE(v_reserved.size() == N);
    REQUIRE(v_unreserved.size() == N);
    
    // Aber Capacity sollte bei reserviertem deutlich gleich N sein
    // (bei unreserviertem möglicherweise größer durch Wachstum)
    REQUIRE(v_reserved.capacity() >= N);
  }
}

// ============================================================
// Test 6: operator[] vs. at()
// ============================================================
TEST_CASE("operator[] vs at() access methods", "[myvector][access]") {
  tfe24::myvector<int> v;
  v.push_back(100);
  v.push_back(200);
  v.push_back(300);

  SECTION("operator[] allows read and write") {
    REQUIRE(v[0] == 100);
    v[1] = 999;
    REQUIRE(v[1] == 999);
  }

  SECTION("at() with valid index") {
    REQUIRE(v.at(0) == 100);
    REQUIRE(v.at(2) == 300);
    
    int& ref = v.at(1);
    ref = 777;
    REQUIRE(v.at(1) == 777);
  }

  SECTION("at() throws on invalid index") {
    REQUIRE_THROWS_AS(v.at(3), std::out_of_range);
    REQUIRE_THROWS_AS(v.at(100), std::out_of_range);
    REQUIRE_THROWS_AS(v.at(size_t(-1)), std::out_of_range);
  }
}

// ============================================================
// Test 7: Template Fähigkeit mit string
// ============================================================
TEST_CASE("myvector works with std::string", "[myvector][template]") {
  tfe24::myvector<std::string> v;
  
  v.push_back("hello");
  v.push_back("world");
  v.push_back("test");
  
  REQUIRE(v.size() == 3);
  REQUIRE(v[0] == "hello");
  REQUIRE(v.at(1) == "world");
  
  SECTION("resize with string default value") {
    v.resize(5);
    REQUIRE(v.size() == 5);
    REQUIRE(v[3] == std::string{});  // Empty string
    REQUIRE(v[4] == std::string{});  // Empty string
  }
}

// ============================================================
// Test 8: Grenzenprüfung und Exception-Sicherheit
// ============================================================
TEST_CASE("bounds checking and exception safety", "[myvector][exceptions]") {
  tfe24::myvector<int> v;
  v.push_back(1);
  v.push_back(2);
  
  SECTION("at() on empty vector throws") {
    tfe24::myvector<int> empty;
    REQUIRE_THROWS_AS(empty.at(0), std::out_of_range);
  }

  SECTION("exception message can be retrieved") {
    try {
      v.at(10);
      FAIL("Expected std::out_of_range exception");
    } catch (const std::out_of_range& e) {
      std::string msg = e.what();
      REQUIRE(!msg.empty());
    }
  }
}

// ============================================================
// Test 9: Speicherverwaltung und Lifecycle
// ============================================================
TEST_CASE("memory management lifecycle", "[myvector][memory]") {
  SECTION("destructor called on scope exit") {
    {
      tfe24::myvector<int> v;
      v.push_back(1);
      v.push_back(2);
      v.push_back(3);
      // Destruktor wird automatisch aufgerufen
    }
    // Kein Memory Leak sollte auftreten
  }

  SECTION("clear and reuse") {
    tfe24::myvector<int> v;
    v.reserve(50);
    for (int i = 0; i < 10; ++i) {
      v.push_back(i);
    }
    
    size_t cap_before = v.capacity();
    v.clear();
    
    REQUIRE(v.size() == 0);
    REQUIRE(v.capacity() == cap_before);  // Capacity erhalten
    
    // Kann wieder verwendet werden
    v.push_back(100);
    REQUIRE(v.size() == 1);
    REQUIRE(v[0] == 100);
  }
}

// ============================================================
// Test 10: Edge Cases
// ============================================================
TEST_CASE("edge cases and corner scenarios", "[myvector][edge]") {
  SECTION("single element operations") {
    tfe24::myvector<int> v;
    v.push_back(42);
    REQUIRE(v.size() == 1);
    REQUIRE(v.at(0) == 42);
    v.clear();
    REQUIRE(v.size() == 0);
  }

  SECTION("large vector operations") {
    tfe24::myvector<int> v;
    int N = 10000;
    for (int i = 0; i < N; ++i) {
      v.push_back(i);
    }
    REQUIRE(v.size() == N);
    REQUIRE(v[0] == 0);
    REQUIRE(v[N-1] == N-1);
  }

  SECTION("alternating push and resize") {
    tfe24::myvector<int> v;
    v.push_back(1);
    v.resize(5);
    v.push_back(2);
    REQUIRE(v.size() == 6);
  }
}



// Compile & run:
// - g++ -std=c++11 -Wall -I$(CATCH_SINGLE_INCLUDE) -o 010-TestCase 010-TestCase.cpp && 010-TestCase --success
// - cl -EHsc -I%CATCH_SINGLE_INCLUDE% 010-TestCase.cpp && 010-TestCase --success

// Expected compact output (all assertions):
//
// prompt> 010-TestCase --reporter compact --success
// 010-TestCase.cpp:14: failed: Factorial(0) == 1 for: 0 == 1
// 010-TestCase.cpp:18: passed: Factorial(1) == 1 for: 1 == 1
// 010-TestCase.cpp:19: passed: Factorial(2) == 2 for: 2 == 2
// 010-TestCase.cpp:20: passed: Factorial(3) == 6 for: 6 == 6
// 010-TestCase.cpp:21: passed: Factorial(10) == 3628800 for: 3628800 (0x375f00) == 3628800 (0x375f00)
// Failed 1 test case, failed 1 assertion.