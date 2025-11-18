#include <catch2/catch_all.hpp>
#include "myvector.hpp"

// ============================================================
// Test Suite 1: Basic Operations
// ============================================================
TEST_CASE("basic operations on tfe24::myvector<int>", "[myvector][basic]") {
    tfe24::myvector<int> v;
    REQUIRE(v.size() == 0);
    REQUIRE(v.capacity() == 0);

    SECTION("push_back increases size") {
        v.push_back(42);
        v.push_back(7);
        REQUIRE(v.size() == 2);
        REQUIRE(v[0] == 42);
        REQUIRE(v.at(1) == 7);
    }

    SECTION("resize to larger fills with default values") {
        v.resize(5);
        REQUIRE(v.size() == 5);
        for (std::size_t i = 0; i < v.size(); ++i) {
            REQUIRE(v[i] == int{});
        }
    }

    SECTION("resize to smaller reduces size") {
        v.push_back(1);
        v.push_back(2);
        v.push_back(3);
        v.resize(1);
        REQUIRE(v.size() == 1);
        REQUIRE(v[0] == 1);
    }

    SECTION("at() throws on out of range") {
        v.push_back(1);
        REQUIRE_THROWS_AS(v.at(5), std::out_of_range);
    }

    SECTION("clear resets size but retains capacity") {
        v.push_back(1);
        v.push_back(2);
        v.push_back(3);
        std::size_t old_capacity = v.capacity();
        v.clear();
        REQUIRE(v.size() == 0);
        REQUIRE(v.capacity() == old_capacity);
    }
}

// ============================================================
// Test Suite 2: Copy Semantics (Rule of Three)
// ============================================================
TEST_CASE("copy semantics (Rule of Three)", "[myvector][copy]") {
    tfe24::myvector<int> original;
    for (int i = 0; i < 3; ++i) original.push_back(i + 10);
    
    SECTION("copy constructor creates independent copy") {
        tfe24::myvector<int> copy_constr = original;
        REQUIRE(copy_constr.size() == original.size());
        REQUIRE(copy_constr.size() == 3);
        
        original[0] = 99;
        REQUIRE(copy_constr[0] != 99);
        REQUIRE(copy_constr[0] == 10);
    }

    SECTION("copy assignment creates independent copy") {
        tfe24::myvector<int> copy_assign;
        copy_assign = original;
        REQUIRE(copy_assign.size() == original.size());
        REQUIRE(copy_assign.size() == 3);
        
        original[1] = 88;
        REQUIRE(copy_assign[1] != 88);
        REQUIRE(copy_assign[1] == 11);
    }

    SECTION("self-assignment is safe") {
        original = original;
        REQUIRE(original.size() == 3);
        REQUIRE(original[0] == 10);
    }
}

// ============================================================
// Test Suite 3: Growth Behavior
// ============================================================
TEST_CASE("capacity doubling on reallocation", "[myvector][growth]") {
    tfe24::myvector<int> v;
    
    v.reserve(4);
    REQUIRE(v.capacity() == 4);
    
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.push_back(4);
    REQUIRE(v.size() == 4);
    REQUIRE(v.capacity() == 4);
    
    // Next push should trigger doubling
    v.push_back(5);
    REQUIRE(v.capacity() == 8);
    REQUIRE(v.size() == 5);
}

// ============================================================
// Test Suite 4: Constructor with Size
// ============================================================
TEST_CASE("constructor with size parameter", "[myvector][constructor]") {
    std::size_t initial_size = 10;
    tfe24::myvector<int> v(initial_size);
    
    REQUIRE(v.size() == initial_size);
    REQUIRE(v.capacity() >= initial_size);
    
    SECTION("elements initialized to default value") {
        for (std::size_t i = 0; i < v.size(); ++i) {
            REQUIRE(v[i] == int{});
        }
    }
}

// ============================================================
// Test Suite 5: Reserve Functionality
// ============================================================
TEST_CASE("reserve allocates space without changing size", 
          "[myvector][reserve]") {
    tfe24::myvector<int> v;
    REQUIRE(v.size() == 0);
    REQUIRE(v.capacity() == 0);

    SECTION("reserve increases capacity") {
        v.reserve(50);
        REQUIRE(v.capacity() >= 50);
        REQUIRE(v.size() == 0);
    }

    SECTION("reserve does not initialize elements") {
        v.push_back(10);
        v.push_back(20);
        std::size_t old_size = v.size();
        
        v.reserve(100);
        REQUIRE(v.size() == old_size);
        REQUIRE(v[0] == 10);
        REQUIRE(v[1] == 20);
    }

    SECTION("reserve with smaller capacity has no effect") {
        v.reserve(100);
        std::size_t cap_100 = v.capacity();
        
        v.reserve(50);
        REQUIRE(v.capacity() == cap_100);
    }

    SECTION("reserve enables efficient batch insertion") {
        tfe24::myvector<int> v_res, v_no_res;
        
        int N = 1000;
        v_res.reserve(N);
        
        for (int i = 0; i < N; ++i) {
            v_res.push_back(i);
            v_no_res.push_back(i);
        }
        
        REQUIRE(v_res.size() == N);
        REQUIRE(v_no_res.size() == N);
        REQUIRE(v_res.capacity() >= N);
    }
}

// ============================================================
// Test Suite 6: Access Methods
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
    }
}

// ============================================================
// Test Suite 7: Template with std::string
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
        REQUIRE(v[3] == std::string{});
        REQUIRE(v[4] == std::string{});
    }
}

// ============================================================
// Test Suite 8: Exception Safety
// ============================================================
TEST_CASE("bounds checking and exception safety", 
          "[myvector][exceptions]") {
    tfe24::myvector<int> v;
    v.push_back(1);
    v.push_back(2);

    SECTION("at() on empty vector throws") {
        tfe24::myvector<int> empty;
        REQUIRE_THROWS_AS(empty.at(0), std::out_of_range);
    }

    SECTION("exception message is accessible") {
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
// Test Suite 9: Memory Management Lifecycle
// ============================================================
TEST_CASE("memory management lifecycle", "[myvector][memory]") {
    SECTION("destructor called on scope exit") {
        {
            tfe24::myvector<int> v;
            v.push_back(1);
            v.push_back(2);
            v.push_back(3);
        }
        // No memory leak after scope
    }

    SECTION("clear and reuse") {
        tfe24::myvector<int> v;
        v.reserve(50);
        for (int i = 0; i < 10; ++i) {
            v.push_back(i);
        }
        
        std::size_t cap_before = v.capacity();
        v.clear();
        
        REQUIRE(v.size() == 0);
        REQUIRE(v.capacity() == cap_before);
        
        v.push_back(100);
        REQUIRE(v.size() == 1);
        REQUIRE(v[0] == 100);
    }
}

// ============================================================
// Test Suite 10: Edge Cases
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
