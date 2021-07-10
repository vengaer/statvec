#include <catch.hpp>

#include "detectors.h"
#include "statvec.h"

#include <array>
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

TEST_CASE("Vector Insertion Lvalue", "[mutators]") {
    SECTION("Simple Insertion") {
        int ten = 10;
        int twelve = 12;
        statvec<int, 128> vec{1, 2, 3, 4, 5, 6};
        REQUIRE(vec.insert(std::cbegin(vec) + 1, ten)[0] == 10);
        REQUIRE(vec.size() == 7);
        REQUIRE(vec == statvec{1, 10, 2, 3, 4, 5, 6});
        REQUIRE(vec.insert(std::cend(vec), twelve)[0] == 12);
        REQUIRE(vec.size() == 8);
        REQUIRE(vec == statvec{1, 10, 2, 3, 4, 5, 6, 12});
    }
    SECTION("Capacity Reached") {
        statvec vec{0};
        REQUIRE(vec.insert(vec.end(), 13) == vec.end());
    }
}

TEST_CASE("Vector Insertion Rvalue", "[mutators]") {
    SECTION("Simple Insertion") {
        statvec<int, 128> vec{1, 2, 3, 4, 5, 6};
        REQUIRE(vec.insert(std::cbegin(vec) + 1, 10)[0] == 10);
        REQUIRE(vec.size() == 7);
        REQUIRE(vec == statvec{1, 10, 2, 3, 4, 5, 6});
        REQUIRE(vec.insert(std::cend(vec), 12)[0] == 12);
        REQUIRE(vec.size() == 8);
        REQUIRE(vec == statvec{1, 10, 2, 3, 4, 5, 6, 12});
    }
    SECTION("Capacity Reached") {
        statvec vec{0};
        REQUIRE(vec.insert(vec.end(), 14) == vec.end());
    }
    SECTION("Move Detection") {
        statvec<move_detector, 32> vec{move_detector{}, move_detector{}, move_detector{}};
        REQUIRE(vec.insert(std::cbegin(vec) + 1, move_detector{}) != vec.end());
        REQUIRE(vec[0].move_assignments == 0);
        REQUIRE(vec[1].move_assignments == 1);
        REQUIRE(vec[2].move_assignments == 1);
        REQUIRE(vec[3].move_assignments == 1);
    }
}

TEST_CASE("Vector Insertion Count", "[mutators]") {
    SECTION("Simple Insertion") {
        std::array arr{12, 12, 12, 12};
        statvec<int, 128> vec{1, 2, 3};
        REQUIRE(vec.insert(vec.begin() + 1, arr.begin(), arr.end())[0] == 12);
        REQUIRE(vec[0] == 1);
        REQUIRE(vec[1] == 12);
        REQUIRE(vec[2] == 12);
        REQUIRE(vec[3] == 12);
        REQUIRE(vec[4] == 12);
        REQUIRE(vec[5] == 2);
        REQUIRE(vec[6] == 3);
        REQUIRE(vec.size() == 7);
    }
    SECTION("Capacity Reached") {
        std::array arr{12, 12, 12, 12};
        statvec<int, 4> vec{1};
        REQUIRE(vec.insert(vec.begin(), arr.begin(), arr.end()) == vec.end());
    }
    SECTION("Empty Range") {
        std::array arr{12, 12, 12, 12};
        statvec<int, 4> vec{1};
        REQUIRE(vec.insert(vec.begin(), arr.begin(), arr.begin()) == vec.begin());
    }
}

TEST_CASE("Vector Emplacement", "[mutators]") {
    SECTION("Simple Emplacement") {
        statvec<std::pair<int, int>, 128> vec;

        for(int i = 0; i < 4; i++) {
            REQUIRE(vec.emplace_back(i, 0));
            REQUIRE(vec.size() == i + 1);
        }
        REQUIRE(vec.emplace(vec.begin() + 1, 12, 13)[0].first == 12);
    }
    SECTION("Move Detection") {
        statvec<std::pair<move_detector, int>, 128> vec;
        for(int i = 0; i < 4; i++) {
            REQUIRE(vec.emplace_back(move_detector{}, i));
            REQUIRE(vec.size() == i + 1);
        }
        REQUIRE(vec.emplace(vec.begin() + 1, move_detector{}, 77) != vec.end());
        REQUIRE(vec[1].first.moves == 1);
        REQUIRE(vec[1].second == 77);
    }
}

TEST_CASE("Vector Element Erasure", "[mutators]") {
    statvec vec{1,2,3,4};
    REQUIRE(vec.erase(vec.begin())[0] == 2);
    REQUIRE(vec.size() == 3);
    REQUIRE(vec == statvec{2, 3, 4});

    auto it = vec.erase(vec.end() - 1);
    REQUIRE(it == vec.end());
}

TEST_CASE("Vector Element Range Erasure", "[mutators]") {
    statvec vec{1, 2, 3, 4, 5, 6, 7, 8, 9};
    REQUIRE(vec.erase(vec.begin(), vec.begin() + 3)[0] == 4);
    REQUIRE(vec == statvec{4, 5, 6, 7, 8, 9});
    REQUIRE(vec.size() == 6);
    auto it = vec.erase(vec.begin(), vec.end());
    REQUIRE(it == vec.end());
    REQUIRE(vec.size() == 0);
}
