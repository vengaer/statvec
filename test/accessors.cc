#include <catch.hpp>

#include "statvec.h"

#include <utility>
#include <type_traits>

template <typename Callable>
bool throws(Callable c) noexcept {
    try {
        c();
    }
    catch(...) {
        return true;
    }
    return false;
}

TEST_CASE("Non-Const Access Operator Access", "[accessors]") {
    statvec vec{1, 2, 3};
    REQUIRE(vec[0] == 1);
    REQUIRE(vec[1] == 2);
    REQUIRE(vec[2] == 3);
}

TEST_CASE("Const Access Operator Access", "[accessors]") {
    statvec const vec{1, 2, 3};
    REQUIRE(vec[0] == 1);
    REQUIRE(vec[1] == 2);
    REQUIRE(vec[2] == 3);
}

TEST_CASE("Non-Const Access Operator Modification", "[accessors]") {
    statvec vec{1, 2, 3};
    vec[2] = 37;
    REQUIRE(vec[2] == 37);
}

TEST_CASE("Const Access Operator Modification Disallowed", "[accessors]") {
    REQUIRE(std::is_const_v<std::remove_reference_t<decltype(std::declval<statvec<int, 4> const>().operator[](0))>>);
}

TEST_CASE("Non-Const Access via at()", "[accessors]") {
    statvec vec{1, 2, 3};
    REQUIRE(vec.at(0) == 1);
    REQUIRE(vec.at(1) == 2);
    REQUIRE(vec.at(2) == 3);
}

TEST_CASE("Const Access via at()", "[accessors]") {
    statvec const vec{1, 2, 3};
    REQUIRE(vec.at(0) == 1);
    REQUIRE(vec.at(1) == 2);
    REQUIRE(vec.at(2) == 3);
}

TEST_CASE("Non-Const Access via at() Modification", "[accessors]") {
    statvec vec{1, 2, 3};
    vec.at(2) = 37;
    REQUIRE(vec.at(2) == 37);
}

TEST_CASE("Const Modification via at() Disallowed", "[accessors]") {
    REQUIRE(std::is_const_v<std::remove_reference_t<decltype(std::declval<statvec<int, 4> const>().at(0))>>);
}

TEST_CASE("Access Beyond End via at() Throws", "[accessors]") {
    REQUIRE(throws([] {
        statvec vec{2};
        vec.at(32);
    }));
}

TEST_CASE("Non-Const Access via front()", "[accessors]") {
    statvec vec{1,2,3,45};
    REQUIRE(vec.front() == 1);
}

TEST_CASE("Const Access via front()", "[accessors]") {
    statvec const vec{1,2,3,45};
    REQUIRE(vec.front() == 1);
}

TEST_CASE("Non-Const Modification via front()", "[accessors]") {
    statvec vec{1,2,3,45};
    vec.front() = 32;
    REQUIRE(vec[0] == 32);
}

TEST_CASE("Const Modification via front() Disallowed", "[accessors]") {
    REQUIRE(std::is_const_v<std::remove_reference_t<decltype(std::declval<statvec<int, 4> const>().front())>>);
}

TEST_CASE("Non-Const Access via back()", "[accessors]") {
    statvec vec{1,2,3,45};
    REQUIRE(vec.back() == 45);
}

TEST_CASE("Const Access via back()", "[accessors]") {
    statvec const vec{1,2,3,45};
    REQUIRE(vec.back() == 45);
}

TEST_CASE("Non-Const Modification via back()", "[accessors]") {
    statvec vec{1,2,3,45};
    vec.back() = 32;
    REQUIRE(vec[3] == 32);
}

TEST_CASE("Const Modification via back() Disallowed", "[accessors]") {
    REQUIRE(std::is_const_v<std::remove_reference_t<decltype(std::declval<statvec<int, 4> const>().back())>>);
}

TEST_CASE("Non-Const Access via data()", "[accessors]") {
    statvec vec{1,2,3};
    int* addr = vec.data();
    REQUIRE(addr[0] == 1);
    REQUIRE(addr[1] == 2);
    REQUIRE(addr[2] == 3);
}

TEST_CASE("Const Access via data()", "[accessors]") {
    statvec const vec{1,2,3};
    int const* addr = vec.data();
    REQUIRE(addr[0] == 1);
    REQUIRE(addr[1] == 2);
    REQUIRE(addr[2] == 3);
}

TEST_CASE("Modification via Non-Const data()", "[accessors]") {
    statvec vec{1,2,3};
    int* addr = vec.data();
    addr[2] = 12;
    REQUIRE(addr[0] == 1);
    REQUIRE(addr[1] == 2);
    REQUIRE(addr[2] == 12);
}

TEST_CASE("Modification via Const data() Disallowed", "[accessors]") {
    REQUIRE(std::is_const_v<std::remove_pointer_t<decltype(std::declval<statvec<int, 4> const>().data())>>);
}
