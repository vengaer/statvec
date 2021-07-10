#include <catch.hpp>

#include "statvec.h"

TEST_CASE("empty()", "[capacity]") {
    {
        statvec vec{1};
        REQUIRE(!vec.empty());
    }
    {
        statvec<int, 3> vec{};
        REQUIRE(vec.empty());
    }
}

TEST_CASE("size()", "[capacity]") {
    {
        statvec<int, 3> vec{};
        REQUIRE(vec.size() == 0);
    }
    {
        statvec vec{1,2,3};
        REQUIRE(vec.size() == 3);
    }
    {
        statvec vec{1,2,3,4,5,6,7,78};
        REQUIRE(vec.size() == 8);
    }
}

TEST_CASE("max_size()", "[capacity]") {
    {
        statvec v{1,2,3};
        REQUIRE(v.max_size() == 3);
    }
    {
        statvec v{1,2,3,4,5};
        REQUIRE(v.max_size() == 5);
    }
    {
        statvec<int, 45> v;
        REQUIRE(v.max_size() == 45);
    }
}

TEST_CASE("capacity()", "[capacity]") {
    {
        statvec v{1,2,3};
        REQUIRE(v.capacity() == 3);
    }
    {
        statvec v{1,2,3,4,5};
        REQUIRE(v.capacity() == 5);
    }
    {
        statvec<int, 45> v;
        REQUIRE(v.capacity() == 45);
    }
}
