#include <catch.hpp>

#include "detectors.h"
#include "statvec.h"

#include <array>
#include <utility>

TEST_CASE("Copy Assignment", "[assignment]") {
    SECTION("Basic Copy Assignment") {
        statvec vec0{1, 2, 3, 4};
        statvec vec1{6, 7, 8, 9};
        statvec<int, 4> vec2{};

        REQUIRE(vec0.size() == 4);
        REQUIRE(vec1.size() == 4);
        REQUIRE(vec2.size() == 0);

        vec1 = vec0;
        REQUIRE(vec1.size() == vec0.size());
        for(unsigned i = 0; i < vec1.size(); i++) {
            REQUIRE(vec0[i] == vec1[i]);
        }

        vec2 = vec0;
        REQUIRE(vec2.size() == vec1.size());
        for(unsigned i = 0; i < vec2.size(); i++) {
            REQUIRE(vec0[i] == vec2[i]);
        }
    }
    SECTION("Copy Detection") {
        statvec vec0{copy_detector{}, copy_detector{}, copy_detector{}};
        statvec<copy_detector, 3> vec1{};

        REQUIRE(vec0.size() == 3);
        REQUIRE(vec1.size() == 0);

        vec1 = vec0;
        REQUIRE(vec1.size() == vec0.size());
        for(unsigned i = 0; i < vec1.size(); i++) {
            REQUIRE(vec1[i].copies == 1);
            REQUIRE(vec1[i].copy_assignments == 1);
        }
    }
    SECTION("Move Detection") {
        statvec vec0{move_detector{}, move_detector{}, move_detector{}};
        statvec<move_detector, 3> vec1{};

        REQUIRE(vec0.size() == 3);
        REQUIRE(vec1.size() == 0);

        vec1 = vec0;
        REQUIRE(vec1.size() == vec0.size());
        for(unsigned i = 0; i < vec1.size(); i++) {
            /* Should be only a single moves internally during assignment */
            REQUIRE(vec1[i].moves == 1);
        }
    }
}

TEST_CASE("Move Assignment", "[assignment]") {
    SECTION("Basic Move Assignment") {
        statvec vec0{1, 2, 3, 4};
        statvec vec1{vec0};
        statvec<int, 4> vec2{};

        REQUIRE(vec0.size() == 4);
        REQUIRE(vec1.size() == 4);
        REQUIRE(vec2.size() == 0);

        vec2 = std::move(vec0);
        REQUIRE(vec1.size() == vec2.size());
        for(unsigned i = 0; i < vec2.size(); i++) {
            REQUIRE(vec1[i] == vec2[i]);
        }
    }
    SECTION("Copy Detection") {
        statvec vec0{copy_detector{}, copy_detector{}, copy_detector{}};
        statvec<copy_detector, 3> vec1{};

        REQUIRE(vec0.size() == 3);
        REQUIRE(vec1.size() == 0);

        vec1 = std::move(vec0);
        REQUIRE(vec1.size() == 3);
        for(unsigned i = 0; i < vec1.size(); i++) {
            REQUIRE(vec1[i].copies == 0);
        }
    }
    SECTION("Move Detection") {
        statvec vec0{move_detector{}, move_detector{}, move_detector{}};
        statvec<move_detector, 3> vec1{};

        REQUIRE(vec0.size() == 3);
        REQUIRE(vec1.size() == 0);

        vec1 = std::move(vec0);
        REQUIRE(vec1.size() == 3);
        for(unsigned i = 0; i < vec1.size(); i++) {
            REQUIRE(vec1[i].moves == 1);
            REQUIRE(vec1[i].move_assignments == 1);
        }
    }
}

TEST_CASE("Array Copy Assignment", "[assignment]") {
    SECTION("Basic Copy Assignment") {
        std::array arr{1, 2, 3, 4};
        statvec<int, 4> vec{};

        REQUIRE(vec.size() == 0);

        vec = arr;
        REQUIRE(vec.size() == arr.size());
        for(unsigned i = 0; i < vec.size(); i++) {
            REQUIRE(vec[i] == arr[i]);
        }
    }
    SECTION("Copy Detection") {
        std::array arr{copy_detector{}, copy_detector{}, copy_detector{}};
        statvec<copy_detector, 3> vec{};

        REQUIRE(vec.size() == 0);

        vec = arr;
        REQUIRE(vec.size() == arr.size());
        for(unsigned i = 0; i < vec.size(); i++) {
            REQUIRE(vec[i].copies == 1);
            REQUIRE(vec[i].copy_assignments == 1);
        }
    }
    SECTION("Move Detection") {
        std::array arr{move_detector{}, move_detector{}, move_detector{}};
        statvec<move_detector, 3> vec{};

        REQUIRE(vec.size() == 0);

        vec = arr;
        REQUIRE(vec.size() == arr.size());
        for(unsigned i = 0; i < vec.size(); i++) {
            REQUIRE(vec[i].moves == 0);
        }
    }
}

TEST_CASE("Array Move Assignment", "[assignment]") {
    SECTION("Basic Move Assignment") {
        std::array arr{1, 2, 3, 4};
        std::array ref{arr};
        statvec<int, 4> vec{};

        REQUIRE(vec.size() == 0);

        vec = std::move(arr);
        REQUIRE(vec.size() == ref.size());
        for(unsigned i = 0; i < vec.size(); i++) {
            REQUIRE(vec[i] == ref[i]);
        }
    }
    SECTION("Copy Detection") {
        std::array arr{copy_detector{}, copy_detector{}, copy_detector{}};
        statvec<copy_detector, 3> vec{};

        REQUIRE(vec.size() == 0);

        vec = std::move(arr);
        REQUIRE(vec.size() == 3);
        for(unsigned i = 0; i < vec.size(); i++) {
            REQUIRE(vec[i].copies == 0);
        }
    }
    SECTION("Move Detection") {
        std::array arr{move_detector{}, move_detector{}, move_detector{}};
        statvec<move_detector, 3> vec{};

        REQUIRE(vec.size() == 0);

        vec = std::move(arr);
        REQUIRE(vec.size() == 3);
        for(unsigned i = 0; i < vec.size(); i++) {
            REQUIRE(vec[i].moves == 1);
            REQUIRE(vec[i].move_assignments == 1);
        }
    }
}
