#include <catch.hpp>

#include "detectors.h"
#include "statvec.h"

#include <array>
#include <utility>
#include <type_traits>

TEST_CASE("Default Construction", "[construction]") {
    statvec<int, 3> vec;
    (void)vec;
}

TEST_CASE("Zero Construction", "[construction]") {
    statvec<int, 3> vec{};
    REQUIRE(vec.size() == 0);
}

TEST_CASE("Variadic Construction", "[construction]") {
    statvec v{1, 2, 3, 4};
    REQUIRE(v[0] == 1);
    REQUIRE(v[1] == 2);
    REQUIRE(v[2] == 3);
    REQUIRE(v[3] == 4);
    REQUIRE(v.size() == 4);
}

TEST_CASE("Array Construction", "[construction]") {
    std::array array{1, 2, 3, 4};
    statvec vec(array);
    REQUIRE(vec.size() == array.size());
    for(unsigned i = 0; i < array.size(); i++) {
        REQUIRE(vec[i] == array[i]);
    }
}

TEST_CASE("Array Copy Construction", "[construction]") {
    SECTION("Copy Detection") {
        std::array<copy_detector, 4> array{};
        statvec vec(array);
        REQUIRE(vec.size() == array.size());
        for(unsigned i = 0; i < vec.size(); i++) {
            REQUIRE(vec[i].copies == 1);
            REQUIRE(vec[i].copy_constructions == 1);
        }
    }
    SECTION("Move Detection") {
        std::array<move_detector, 4> array{};
        statvec vec(array);
        REQUIRE(vec.size() == array.size());
        for(unsigned i = 0; i < vec.size(); i++) {
            REQUIRE(vec[i].moves == 0);
        }
    }
}

TEST_CASE("Array Move Construction", "[construction]") {
    SECTION("Copy Detection") {
        std::array<copy_detector, 4> array{};
        statvec vec(std::move(array));
        REQUIRE(vec.size() == 4);
        for(unsigned i = 0; i < vec.size(); i++) {
            REQUIRE(vec[i].copies == 0);
        }
    }
    SECTION("Move Detection") {
        std::array<move_detector, 4> array{};
        statvec vec(std::move(array));
        REQUIRE(vec.size() == 4);
        for(unsigned i = 0; i < vec.size(); i++) {
            REQUIRE(vec[i].moves == 1);
            REQUIRE(vec[i].move_constructions == 1);
        }
    }
}

TEST_CASE("Copy Construction", "[construction]") {
    SECTION("Basic Copy") {
        statvec vec0{1, 2, 3, 4};
        statvec vec1 = vec0;
        REQUIRE(vec0.size() == vec1.size());
        for(unsigned i = 0; i < vec0.size(); i++) {
            REQUIRE(vec0[i] == vec1[i]);
        }
    }
    SECTION("Copy Detection") {
        statvec vec0{copy_detector{}, copy_detector{}, copy_detector{}, copy_detector{}};
        statvec vec1 = vec0;
        REQUIRE(vec1.size() == 4);
        for(unsigned i = 0; i < vec0.size(); i++) {
            REQUIRE(vec1[i].copies == 1);
            REQUIRE(vec1[i].copy_constructions == 1);
        }
    }
    SECTION("Move Detection") {
        statvec vec0{move_detector{}, move_detector{}, move_detector{}, move_detector{}};
        statvec vec1 = vec0;
        REQUIRE(vec1.size() == 4);
        for(unsigned i = 0; i < vec0.size(); i++) {
            /* Should be only a single move internally during construction */
            REQUIRE(vec1[i].moves == 1);
        }
    }
}

TEST_CASE("Move Construction", "[construction]") {
    SECTION("Basic Move") {
        /* Not that moving ints makes much sense... */
        statvec vec0{1, 2, 3, 4};
        statvec vec1 = vec0;
        statvec vec2 = std::move(vec0);
        REQUIRE(vec2.size() == vec1.size());
        for(unsigned i = 0; i < vec2.size(); i++) {
            REQUIRE(vec1[i] == vec2[i]);
        }
    }
    SECTION("Copy Detection") {
        statvec vec0{copy_detector{}, copy_detector{}, copy_detector{}, copy_detector{}, };
        statvec vec1 = std::move(vec0);
        REQUIRE(vec1.size() == 4);
        for(unsigned i = 0; i < vec0.size(); i++) {
            REQUIRE(vec1[i].copies == 0);
            REQUIRE(vec1[i].copy_constructions == 0);
        }
    }
    SECTION("Move Detection") {
        statvec vec0{move_detector{}, move_detector{}, move_detector{}, move_detector{}};
        statvec vec1 = std::move(vec0);
        REQUIRE(vec1.size() == 4);
        for(unsigned i = 0; i < vec0.size(); i++) {
            REQUIRE(vec1[i].moves == 1);
            REQUIRE(vec1[i].move_constructions == 1);
        }
    }
}

TEST_CASE("Construction Without Intermediate Default Constructed Objects", "[construction]") {
    struct non_default {
        non_default(int i) noexcept
            : i_{i} { }
        int i_{};
    };
    static_assert(!std::is_default_constructible_v<non_default>);
    statvec vec{non_default{10}, non_default{10}};
    REQUIRE(vec.size() == 2);
}
