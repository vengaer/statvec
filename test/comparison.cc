#include <catch.hpp>

#include "statvec.h"

TEST_CASE("Equality Comparison", "[comparison]") {
    REQUIRE(statvec{1,2,3} == statvec{1,2,3});
    REQUIRE(!(statvec{1,2,3} == statvec{1,2}));
    REQUIRE(!(statvec{1,2,3} == statvec{1,2,3,4}));
    REQUIRE(!(statvec{1,2,3} == statvec{1,2,2}));
    REQUIRE(statvec{1,2,3} == statvec<int, 32>{1,2,3});
}

TEST_CASE("Inequality Comparison", "[comparison]") {
    REQUIRE(!(statvec{1,2,3} != statvec{1,2,3}));
    REQUIRE(statvec{1,2,3} != statvec{1,2});
    REQUIRE(statvec{1,2,3} != statvec{1,2,3,4});
    REQUIRE(statvec{1,2,3} != statvec{1,2,2});
    REQUIRE(!(statvec{1,2,3} != statvec<int, 32>{1,2,3}));
}

TEST_CASE("Less Than or Equal Comparison", "[comparison]") {
    REQUIRE(statvec{1,2,3} <= statvec{1,2,3});
    REQUIRE(statvec{1,2,3} <= statvec{1,4});
    REQUIRE(statvec{1,2} <= statvec{1,2,3});
    REQUIRE(!(statvec{1,2,3} <= statvec{1,2}));
    REQUIRE(!(statvec{1,4} <= statvec{1,2,3}));
}

TEST_CASE("Greater Than or Equal Comparison", "[comparison]") {
    REQUIRE(statvec{1,2,3} >= statvec{1,2,3});
    REQUIRE(!(statvec{1,2,3} >= statvec{1,4}));
    REQUIRE(!(statvec{1,2} >= statvec{1,2,3}));
    REQUIRE(statvec{1,2,3} >= statvec{1,2});
    REQUIRE(statvec{1,4} >= statvec{1,2,3});
}

TEST_CASE("Less Than Comparison", "[comparison]") {
    REQUIRE(!(statvec{1,2,3} < statvec{1,2,3}));
    REQUIRE(statvec{1,2,3} < statvec{1,4});
    REQUIRE(statvec{1,2} < statvec{1,2,3});
    REQUIRE(!(statvec{1,2,3} < statvec{1,2}));
    REQUIRE(!(statvec{1,4} < statvec{1,2,3}));
}

TEST_CASE("Greater Than Comparison", "[comparison]") {
    REQUIRE(!(statvec{1,2,3} > statvec{1,2,3}));
    REQUIRE(!(statvec{1,2,3} > statvec{1,4}));
    REQUIRE(!(statvec{1,2} > statvec{1,2,3}));
    REQUIRE(statvec{1,2,3} > statvec{1,2});
    REQUIRE(statvec{1,4} > statvec{1,2,3});
}
