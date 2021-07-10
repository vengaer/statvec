#include <catch.hpp>

#include "detectors.h"
#include "statvec.h"

#include <utility>

TEST_CASE("Insert Lvalue Through push_back", "[mutators]") {
    SECTION("Basic Insertion") {
        int values[128];
        statvec<int, 128> vec{};

        for(int i = 0; i < 128; i++) {
            values[i] = i;
        }

        REQUIRE(vec.size() == 0);
        for(unsigned i = 0u; i  < 128u; i++) {
            REQUIRE(vec.push_back(values[i]));
            REQUIRE(vec[i] == values[i]);
            REQUIRE(vec.size() == i + 1);
        }
        REQUIRE(!vec.push_back(values[0]));
    }
    SECTION("Copy Detection") {
        copy_detector cd{};
        statvec<copy_detector, 128> vec{};
        for(unsigned i = 0; i < 128; i++) {
            REQUIRE(vec.push_back(cd));
        }
        for(unsigned i = 0; i < 128; i++) {
            REQUIRE(vec[i].copies == 1);
            REQUIRE(vec[i].copy_assignments == 1);
        }
    }
    SECTION("Move Detection") {
        move_detector md{};
        statvec<move_detector, 128> vec{};
        for(unsigned i = 0; i < 128; i++) {
            REQUIRE(vec.push_back(md));
        }
        for(unsigned i = 0; i < 128; i++) {
            REQUIRE(vec[i].moves == 0);
        }
    }
}

TEST_CASE("Insert Rvalue Through push_back", "[mutators]") {
    SECTION("Basic Insertion") {
        int values[128];
        statvec<int, 128> vec{};

        for(int i = 0; i < 128; i++) {
            values[i] = i;
        }

        REQUIRE(vec.size() == 0);
        for(unsigned i = 0u; i  < 128u; i++) {
            REQUIRE(vec.push_back(std::move(values[i])));
            REQUIRE(vec[i] == values[i]);
            REQUIRE(vec.size() == i + 1);
        }
        REQUIRE(!vec.push_back(std::move(values[0])));
    }
    SECTION("Copy Detection") {
        statvec<copy_detector, 128> vec{};
        for(unsigned i = 0; i < 128; i++) {
            REQUIRE(vec.push_back(copy_detector{}));
        }
        for(unsigned i = 0; i < 128; i++) {
            REQUIRE(vec[i].copies == 0);
        }
    }
    SECTION("Move Detection") {
        statvec<move_detector, 128> vec{};
        for(unsigned i = 0; i < 128; i++) {
            REQUIRE(vec.push_back(move_detector{}));
        }
        for(unsigned i = 0; i < 128; i++) {
            REQUIRE(vec[i].moves == 1);
            REQUIRE(vec[i].move_assignments == 1);
        }
    }
}

TEST_CASE("Insertion via emplace_back", "[mutators]") {
    SECTION("Basic Emplacement") {
        statvec<std::pair<int, int>, 128> vec;

        for(int i = 0; i < 128; i++) {
            REQUIRE(vec.emplace_back(i, 0));
            REQUIRE(vec.size() == i + 1);
            REQUIRE(vec[i].first == i);
            REQUIRE(vec[i].second == 0);
        }
        REQUIRE(!vec.emplace_back(0, 0));
    }
    SECTION("Intermediate Detection") {
        statvec<forward_detector, 128> vec{};

        REQUIRE(vec.emplace_back(0, 0));
        REQUIRE(vec[0].constructions == 1);
    }
}

TEST_CASE("Removal via pop_back()", "[mutators]") {
    statvec vec{1, 2, 3, 4};
    REQUIRE(vec.size() == 4);
    REQUIRE(vec.pop_back() == 4);
    REQUIRE(vec.size() == 3);
    REQUIRE(vec.pop_back() == 3);
    REQUIRE(vec.size() == 2);
    REQUIRE(vec.pop_back() == 2);
    REQUIRE(vec.size() == 1);
    REQUIRE(vec.pop_back() == 1);
    REQUIRE(vec.size() == 0);
}

TEST_CASE("Vector Swapping", "[mutators]") {
    statvec vec0{1,2,3};
    statvec vec1{4,5,6};
    std::swap(vec0, vec1);
    for(unsigned i = 0; i < vec0.size(); i++) {
        REQUIRE(vec1[i] == vec0[i] - 3);
    }
}

TEST_CASE("Vector Clearing", "[mutators]") {
    statvec vec{1,2,3};
    REQUIRE(vec.size() == 3);
    vec.clear();
    REQUIRE(vec.size() == 0);
}

TEST_CASE("Vector resizing", "[mutators]") {
    statvec<int, 128> vec{1,2,3};
    REQUIRE(vec.size() == 3);
    REQUIRE(vec.resize(32));
    REQUIRE(vec.size() == 32);
    REQUIRE(vec.resize(0));
    REQUIRE(vec.size() == 0);
    REQUIRE(vec.empty());
    REQUIRE(!vec.resize(vec.capacity() + 1));
    REQUIRE(vec.size() == vec.capacity());
}
