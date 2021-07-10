#include <catch.hpp>

#include "detectors.h"
#include "statvec.h"

TEST_CASE("Lvalue reserve()", "[reserve]") {
    SECTION("Basic Resizing") {
        statvec vec0{1,2,3,4};
        auto vec1 = vec0.template reserve<32>();
        REQUIRE(vec0.capacity() == 4);
        REQUIRE(vec1.capacity() == 32);

        REQUIRE(vec1[0] == 1);
        REQUIRE(vec1[3] == 4);
    }
    SECTION("Copy Detection") {
        statvec vec0{copy_detector{}, copy_detector{}};
        REQUIRE(vec0.capacity() == 2);
        auto vec1 = vec0.template reserve<32>();
        REQUIRE(vec1[0].copies == 1);
    }
    SECTION("Move Detection") {
        statvec vec0{move_detector{}, move_detector{}};
        REQUIRE(vec0.capacity() == 2);
        auto vec1 = vec0.template reserve<32>();
        /* One move is expected during construction */
        REQUIRE(vec1[0].moves == 1);
    }
}

TEST_CASE("Rvalue reserve()", "[reserve]") {
    SECTION("Basic Resizing") {
        statvec vec0{1,2,3,4};
        REQUIRE(vec0.capacity() == 4);
        auto vec1 = std::move(vec0).template reserve<32>();
        REQUIRE(vec1.capacity() == 32);
        REQUIRE(vec1[0] == 1);
        REQUIRE(vec1[3] == 4);
    }
    SECTION("Copy Detection") {
        statvec vec0{copy_detector{}, copy_detector{}};
        REQUIRE(vec0.capacity() == 2);
        auto vec1 = std::move(vec0).template reserve<32>();
        REQUIRE(vec1[0].copies == 0);
    }
    SECTION("Move Detection") {
        statvec vec0{move_detector{}, move_detector{}};
        REQUIRE(vec0.capacity() == 2);
        auto vec1 = std::move(vec0).template reserve<32>();
        REQUIRE(vec1[0].moves == 1);
    }
}

TEST_CASE("Lvalue Resize via statvec_reserve", "[reserve]") {
    SECTION("Basic Resizing") {
        statvec vec0{1,2,3,4};
        REQUIRE(vec0.capacity() == 4);
        auto vec1 = statvec_reserve<32>(vec0);
        REQUIRE(vec1.capacity() == 32);
        REQUIRE(vec1[0] == 1);
        REQUIRE(vec1[3] == 4);
    }
    SECTION("Copy Detection") {
        statvec vec0{copy_detector{}, copy_detector{}};
        REQUIRE(vec0.capacity() == 2);
        auto vec1 = statvec_reserve<32>(vec0);
        REQUIRE(vec1.capacity() == 32);
        REQUIRE(vec1[0].copies == 1);
    }
    SECTION("Move Detection") {
        statvec vec0{move_detector{}, move_detector{}};
        REQUIRE(vec0.capacity() == 2);
        auto vec1 = statvec_reserve<32>(vec0);
        REQUIRE(vec1.capacity() == 32);
        /* One move is expected during construction */
        REQUIRE(vec1[0].moves == 1);
    }
}

TEST_CASE("Rvalue Resize via statvec_reserve", "[reserve]") {
    SECTION("Basic Resizing") {
        statvec vec0{1,2,3,4};
        REQUIRE(vec0.capacity() == 4);
        auto vec1 = statvec_reserve<32>(std::move(vec0));
        REQUIRE(vec1.capacity() == 32);
        REQUIRE(vec1[0] == 1);
        REQUIRE(vec1[3] == 4);
    }
    SECTION("Copy Detection") {
        statvec vec0{copy_detector{}, copy_detector{}};
        REQUIRE(vec0.capacity() == 2);
        auto vec1 = statvec_reserve<32>(std::move(vec0));
        REQUIRE(vec1.capacity() == 32);
        REQUIRE(vec1[0].copies == 0);
    }
    SECTION("Move Detection") {
        statvec vec0{move_detector{}, move_detector{}};
        REQUIRE(vec0.capacity() == 2);
        auto vec1 = statvec_reserve<32>(std::move(vec0));
        REQUIRE(vec1.capacity() == 32);
        REQUIRE(vec1[0].moves == 1);
    }
}
