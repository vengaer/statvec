#include <catch.hpp>

#include "statvec.h"

#include <cstddef>
#include <type_traits>

static std::size_t constexpr vecsize = 256;

using vec_type  = statvec<int, vecsize>;
using cvec_type = vec_type const;

using iter_type   = decltype(std::declval<vec_type>().begin());
using citer_type  = decltype(std::declval<vec_type>().cbegin());
using riter_type  = decltype(std::declval<vec_type>().rbegin());
using criter_type = decltype(std::declval<vec_type>().crbegin());

struct compound_type {
    compound_type() = default;
    compound_type(int a, int b) : i{a}, j{b} { }
    int i, j;
};

using compound_vec  = statvec<compound_type, vecsize>;
using ccompound_vec = compound_vec const;

template <typename>
struct is_const_iterator : std::false_type { };

template <>
struct is_const_iterator<citer_type> : std::true_type { };

template <>
struct is_const_iterator<criter_type> : std::true_type { };

template <typename T>
inline bool constexpr is_const_iterator_v = is_const_iterator<T>::value;

TEMPLATE_TEST_CASE("Iterator Default Construction", "[iterator]", iter_type, citer_type, riter_type, criter_type) {
    TestType it;
    (void)it;
}

TEMPLATE_TEST_CASE("Iterator Copy Construction", "[iterator]", iter_type, citer_type, riter_type, criter_type) {
    TestType it0{};
    TestType it1{it0};
    REQUIRE(it0 == it1);
}

TEST_CASE("Const Iterator Construction from Non-Const", "[iterator]") {
    vec_type vec{1,2,3};
    SECTION("const_iterator") {
        decltype(vec.cbegin()) cit = vec.begin();
        REQUIRE(*cit == 1);
    }
    SECTION("const_reverse_iterator") {
        decltype(vec.crbegin()) cit = vec.rbegin();
        REQUIRE(*cit == 3);
    }
}

TEMPLATE_TEST_CASE("Iterator Copy Assignment", "[iterator]", iter_type, citer_type, riter_type, criter_type) {
    TestType it0{};
    TestType it1{};
    it0 = it1;
    REQUIRE(it0 == it1);
}

TEST_CASE("Const Iterator Assignment from Non-Const", "[iterator]") {
    vec_type vec{1,2,3};
    SECTION("const_iterator") {
        auto cit = vec.cend();
        cit = vec.begin();
        REQUIRE(*cit == 1);
    }
    SECTION("const_reverse_iterator") {
        auto cit = vec.crend();
        cit = vec.rbegin();
        REQUIRE(*cit == 3);
    }
}

TEMPLATE_TEST_CASE("Qualifier for Iterator Returned by begin()", "[iterator]", vec_type, cvec_type) {
    REQUIRE(std::is_const_v<TestType> == std::is_same_v<citer_type, decltype(std::declval<TestType>().begin())>);
}

TEMPLATE_TEST_CASE("Qualifier for Iterator Returned by end()", "[iterator]", vec_type, cvec_type) {
    REQUIRE(std::is_const_v<TestType> == std::is_same_v<citer_type, decltype(std::declval<TestType>().end())>);
}

TEMPLATE_TEST_CASE("Qualifier for Iterator Returned by rbegin()", "[iterator]", vec_type, cvec_type) {
    REQUIRE(std::is_const_v<TestType> == std::is_same_v<criter_type, decltype(std::declval<TestType>().rbegin())>);
}

TEMPLATE_TEST_CASE("Qualifier for Iterator Returned by rend()", "[iterator]", vec_type, cvec_type) {
    REQUIRE(std::is_const_v<TestType> == std::is_same_v<criter_type, decltype(std::declval<TestType>().rend())>);
}

TEMPLATE_TEST_CASE("Qualifier for Iterator Dereferencing", "[iterator]", iter_type, citer_type, riter_type, criter_type) {
    REQUIRE(is_const_iterator_v<TestType> == std::is_const_v<std::remove_reference_t<decltype(std::declval<TestType>().operator*())>>);
}

TEMPLATE_TEST_CASE("Qualifier for Iterator Access Operator", "[iterator]", iter_type, citer_type, riter_type, criter_type) {
    REQUIRE(is_const_iterator_v<TestType> == std::is_const_v<std::remove_reference_t<decltype(std::declval<TestType>().operator[](std::declval<typename TestType::difference_type>()))>>);
}

TEMPLATE_TEST_CASE("Qualifier for Iterator Arrow Operator", "[iterator]", iter_type, citer_type, riter_type, criter_type) {
    REQUIRE(is_const_iterator_v<TestType> == std::is_const_v<std::remove_pointer_t<decltype(std::declval<TestType>().operator->())>>);
}

TEMPLATE_TEST_CASE("Value Access Through Iterator Dereferenceing", "[iterator]", vec_type, cvec_type) {
    TestType vec{1,2,3,4,5,6,7,8};
    REQUIRE(*vec.begin() == 1);
    REQUIRE(*vec.cbegin() == 1);
    REQUIRE(*vec.rbegin() == 8);
    REQUIRE(*vec.crbegin() == 8);
}

TEST_CASE("Value Modification Through Iterator Dereferencing", "[iterator]") {
    vec_type vec{1,2,3};
    *vec.begin() = 10;
    REQUIRE(vec[0] == 10);
    *vec.rbegin() = 8;
    REQUIRE(vec[2] == 8);
}

TEMPLATE_TEST_CASE("Value Access Through Iterator Access Operator", "[iterator]", vec_type, cvec_type) {
    TestType vec{1,2,3,4,5,6,7,8};
    REQUIRE(vec.begin()[0] == 1);
    REQUIRE(vec.cbegin()[0] == 1);
    REQUIRE(vec.rbegin()[0] == 8);
    REQUIRE(vec.crbegin()[0] == 8);
    REQUIRE(vec.begin()[1] == 2);
    REQUIRE(vec.cbegin()[1] == 2);
    REQUIRE(vec.rbegin()[1] == 7);
    REQUIRE(vec.crbegin()[1] == 7);
}

TEST_CASE("Value Modification Through Iterator Access Operator", "[iterator]") {
    vec_type vec{1,2,3};
    vec.begin()[0] = 10;
    REQUIRE(vec[0] == 10);
    vec.rbegin()[1] = 11;
    REQUIRE(vec[1] == 11);
}

TEMPLATE_TEST_CASE("Value Access Through Iterator Arrow Operator", "[iterator]", compound_vec, ccompound_vec) {
    std::array<compound_type, 4> a;
    a[0].i = 0;
    a[0].j = 0;
    a[1].i = 1;
    a[1].j = 1;
    a[2].i = 2;
    a[2].j = 2;
    a[3].i = 3;
    a[3].j = 3;
    TestType vec{a};

    REQUIRE(vec.begin()->i == 0);
    REQUIRE(vec.rbegin()->i == 3);
    REQUIRE(vec.cbegin()->i == 0);
    REQUIRE(vec.crbegin()->i == 3);
}

TEST_CASE("Value Modification Through Iterator Arrow Operator", "[iterator]") {
    compound_vec vec;
    REQUIRE(vec.emplace_back(0, 0));
    REQUIRE(vec.emplace_back(1, 1));
    REQUIRE(vec.emplace_back(2, 2));
    vec.begin()->i = 12;
    REQUIRE(vec[0].i == 12);
    vec.rbegin()->j = 3;
    REQUIRE(vec[2].j == 3);
}

TEMPLATE_TEST_CASE("Iterator Pre-Increment", "[iterator]", vec_type, cvec_type) {
    TestType vec{1,2,3,4};
    SECTION("iterator") {
        auto it = std::begin(vec);
        REQUIRE(*it == 1);
        ++it;
        REQUIRE(*it == 2);
    }
    SECTION("const_iterator") {
        auto cit = std::cbegin(vec);
        REQUIRE(*cit == 1);
        ++cit;
        REQUIRE(*cit == 2);
    }
    SECTION("reverse_iterator") {
        auto rit = std::rbegin(vec);
        REQUIRE(*rit == 4);
        ++rit;
        REQUIRE(*rit == 3);
    }
    SECTION("const_reverse_iterator") {
        auto crit = std::crbegin(vec);
        REQUIRE(*crit == 4);
        ++crit;
        REQUIRE(*crit == 3);
    }
}

TEMPLATE_TEST_CASE("Iterator Post-Increment", "[iterator]", vec_type, cvec_type) {
    TestType vec{1,2,3,4};
    SECTION("iterator") {
        auto it = std::begin(vec);
        REQUIRE(*it++ == 1);
        REQUIRE(*it == 2);
    }
    SECTION("const_iterator") {
        auto cit = std::cbegin(vec);
        REQUIRE(*cit++ == 1);
        REQUIRE(*cit == 2);
    }
    SECTION("reverse_iterator") {
        auto rit = std::rbegin(vec);
        REQUIRE(*rit++ == 4);
        REQUIRE(*rit == 3);
    }
    SECTION("const_reverse_iterator") {
        auto crit = std::crbegin(vec);
        REQUIRE(*crit++ == 4);
        REQUIRE(*crit == 3);
    }
}

TEMPLATE_TEST_CASE("Iterator Pre-Decrement", "[iterator]", vec_type, cvec_type) {
    TestType vec{1,2,3,4};
    SECTION("iterator") {
        auto it = std::end(vec);
        --it;
        REQUIRE(*it == 4);
        --it;
        REQUIRE(*it == 3);
    }
    SECTION("const_iterator") {
        auto cit = std::cend(vec);
        --cit;
        REQUIRE(*cit == 4);
        --cit;
        REQUIRE(*cit == 3);
    }
    SECTION("reverse_iterator") {
        auto rit = std::rend(vec);
        --rit;
        REQUIRE(*rit == 1);
        --rit;
        REQUIRE(*rit == 2);
    }
    SECTION("const_reverse_iterator") {
        auto crit = std::crend(vec);
        --crit;
        REQUIRE(*crit == 1);
        --crit;
        REQUIRE(*crit == 2);
    }
}

TEMPLATE_TEST_CASE("Iterator Post-Decrement", "[iterator]", vec_type, cvec_type) {
    TestType vec{1,2,3,4};
    SECTION("iterator") {
        auto it = std::end(vec);
        --it;
        REQUIRE(*it-- == 4);
        REQUIRE(*it == 3);
    }
    SECTION("const_iterator") {
        auto cit = std::cend(vec);
        --cit;
        REQUIRE(*cit-- == 4);
        REQUIRE(*cit == 3);
    }
    SECTION("reverse_iterator") {
        auto rit = std::rend(vec);
        --rit;
        REQUIRE(*rit-- == 1);
        REQUIRE(*rit == 2);
    }
    SECTION("const_reverse_iterator") {
        auto crit = std::crend(vec);
        --crit;
        REQUIRE(*crit-- == 1);
        REQUIRE(*crit == 2);
    }
}

TEMPLATE_TEST_CASE("Iterator Advance", "[iterator]", vec_type, cvec_type) {
    TestType vec{1,2,3,4};
    SECTION("iterator") {
        auto it = std::begin(vec);
        REQUIRE(*it == 1);
        it += 2;
        REQUIRE(*it == 3);
    }
    SECTION("const_iterator") {
        auto cit = std::cbegin(vec);
        REQUIRE(*cit == 1);
        cit += 2;
        REQUIRE(*cit == 3);
    }
    SECTION("reverse_iterator") {
        auto rit = std::rbegin(vec);
        REQUIRE(*rit == 4);
        rit += 2;
        REQUIRE(*rit == 2);
    }
    SECTION("const_reverse_iterator") {
        auto crit = std::crbegin(vec);
        REQUIRE(*crit == 4);
        crit += 2;
        REQUIRE(*crit == 2);
    }
}

TEMPLATE_TEST_CASE("Iterator Retract", "[iterator]", vec_type, cvec_type) {
    TestType vec{1,2,3,4};
    SECTION("iterator") {
        auto it = std::end(vec);
        --it;
        REQUIRE(*it == 4);
        it -= 2;
        REQUIRE(*it == 2);
    }
    SECTION("const_iterator") {
        auto cit = std::cend(vec);
        --cit;
        REQUIRE(*cit == 4);
        cit -= 2;
        REQUIRE(*cit == 2);
    }
    SECTION("reverse_iterator") {
        auto rit = std::rend(vec);
        --rit;
        REQUIRE(*rit == 1);
        rit -= 2;
        REQUIRE(*rit == 3);
    }
    SECTION("const_reverse_iterator") {
        auto crit = std::crend(vec);
        --crit;
        REQUIRE(*crit == 1);
        crit -= 2;
        REQUIRE(*crit == 3);
    }
}

TEMPLATE_TEST_CASE("Iterator Addition", "[iterator]", vec_type, cvec_type) {
    TestType vec{1,2,3,4};
    SECTION("iterator") {
        auto it = vec.begin();
        it = it + 2;
        REQUIRE(*it == 3);
        it = 1 + it;
        REQUIRE(*it == 4);
    }
    SECTION("const_iterator") {
        auto it = vec.cbegin();
        it = it + 2;
        REQUIRE(*it == 3);
        it = 1 + it;
        REQUIRE(*it == 4);
    }
    SECTION("reverse_iterator") {
        auto it = vec.rbegin();
        it = it + 2;
        REQUIRE(*it == 2);
        it = 1 + it;
        REQUIRE(*it == 1);
    }
    SECTION("const_reverse_iterator") {
        auto it = vec.crbegin();
        it = it + 2;
        REQUIRE(*it == 2);
        it = 1 + it;
        REQUIRE(*it == 1);
    }
}

TEMPLATE_TEST_CASE("Iterator Swap", "[iterator]", vec_type, cvec_type) {
    SECTION("iterator") {
        TestType vec{1,2,3,4};
        auto it0 = std::begin(vec);
        auto it1 = std::end(vec);
        std::swap(it0, it1);
        REQUIRE(it0 == std::end(vec));
        REQUIRE(it1 == std::begin(vec));
    }
    SECTION("const_iterator") {
        TestType vec{1,2,3,4};
        auto it0 = std::cbegin(vec);
        auto it1 = std::cend(vec);
        std::swap(it0, it1);
        REQUIRE(it0 == std::cend(vec));
        REQUIRE(it1 == std::cbegin(vec));
    }
    SECTION("reverse_iterator") {
        TestType vec{1,2,3,4};
        auto it0 = std::rbegin(vec);
        auto it1 = std::rend(vec);
        std::swap(it0, it1);
        REQUIRE(it0 == std::rend(vec));
        REQUIRE(it1 == std::rbegin(vec));
    }
    SECTION("const_reverse_iterator") {
        TestType vec{1,2,3,4};
        auto it0 = std::crbegin(vec);
        auto it1 = std::crend(vec);
        std::swap(it0, it1);
        REQUIRE(it0 == std::crend(vec));
        REQUIRE(it1 == std::crbegin(vec));
    }
}

TEMPLATE_TEST_CASE("Iterator Integral Subtraction", "[iterator]", vec_type, cvec_type) {
    TestType vec{1,2,3,4};
    SECTION("iterator") {
        auto it = vec.end();
        it = it - 2;
        REQUIRE(*it == 3);
    }
    SECTION("const_iterator") {
        auto it = vec.cend();
        it = it - 2;
        REQUIRE(*it == 3);
    }
    SECTION("reverse_iterator") {
        auto it = vec.rend();
        it = it - 2;
        REQUIRE(*it == 2);
    }
    SECTION("const_reverse_iterator") {
        auto it = vec.crend();
        it = it - 2;
        REQUIRE(*it == 2);
    }
}

TEMPLATE_TEST_CASE("Iterator Subtraction", "[iterator]", vec_type, cvec_type) {
    TestType vec{1,2,3,4};
    SECTION("iterator") {
        REQUIRE(vec.end() - vec.begin() == (int)vec.size());
    }
    SECTION("const_iterator") {
        REQUIRE(vec.cend() - vec.cbegin() == (int)vec.size());
    }
    SECTION("reverse_iterator") {
        REQUIRE(vec.rend() - vec.rbegin() == (int)vec.size());
    }
    SECTION("const_reverse_iterator") {
        REQUIRE(vec.crend() - vec.crbegin() == (int)vec.size());
    }
}

TEMPLATE_TEST_CASE("Iterator Equality Comparison", "[iterator]", iter_type, citer_type, riter_type, criter_type) {
    TestType it0{};
    TestType it1{it0};
    REQUIRE(it0 == it1);
}

TEMPLATE_TEST_CASE("Iterator Inequality Comparison", "[iterator]", vec_type, cvec_type) {
    TestType vec{1,2,3,4};
    SECTION("iterator") {
        REQUIRE(vec.begin() != vec.end());
    }
    SECTION("const_iterator") {
        REQUIRE(vec.cbegin() != vec.cend());
    }
    SECTION("reverse_iterator") {
        REQUIRE(vec.rbegin() != vec.rend());
    }
    SECTION("const_reverse_iterator") {
        REQUIRE(vec.crbegin() != vec.crend());
    }
}

TEMPLATE_TEST_CASE("Iterator Less Than or Equal Comparison", "[iterator]", vec_type, cvec_type) {
    TestType vec{1,2,3,4};
    SECTION("iterator") {
        REQUIRE(vec.begin() <= vec.begin());
        REQUIRE(vec.begin() <= vec.end());
    }
    SECTION("const_iterator") {
        REQUIRE(vec.cbegin() <= vec.cbegin());
        REQUIRE(vec.cbegin() <= vec.cend());
    }
    SECTION("reverse_iterator") {
        REQUIRE(vec.rbegin() <= vec.rbegin());
        REQUIRE(vec.rbegin() <= vec.rend());
    }
    SECTION("const_reverse_iterator") {
        REQUIRE(vec.crbegin() <= vec.crbegin());
        REQUIRE(vec.crbegin() <= vec.crend());
    }
}

TEMPLATE_TEST_CASE("Iterator Greater Than or Equal Comparison", "[iterator]", vec_type, cvec_type) {
    TestType vec{1,2,3,4};
    SECTION("iterator") {
        REQUIRE(vec.begin() >= vec.begin());
        REQUIRE(vec.end() >= vec.begin());
    }
    SECTION("const_iterator") {
        REQUIRE(vec.cbegin() >= vec.cbegin());
        REQUIRE(vec.cend() >= vec.cbegin());
    }
    SECTION("reverse_iterator") {
        REQUIRE(vec.rbegin() >= vec.rbegin());
        REQUIRE(vec.rend() >= vec.rbegin());
    }
    SECTION("const_reverse_iterator") {
        REQUIRE(vec.crbegin() >= vec.crbegin());
        REQUIRE(vec.crend() >= vec.crbegin());
    }
}

TEMPLATE_TEST_CASE("Iterator Less Than Comparison", "[iterator]", vec_type, cvec_type) {
    TestType vec{1,2,3,4};
    SECTION("iterator") {
        REQUIRE(vec.begin() < vec.end());
    }
    SECTION("const_iterator") {
        REQUIRE(vec.cbegin() < vec.cend());
    }
    SECTION("reverse_iterator") {
        REQUIRE(vec.rbegin() < vec.rend());
    }
    SECTION("const_reverse_iterator") {
        REQUIRE(vec.crbegin() < vec.crend());
    }
}

TEMPLATE_TEST_CASE("Iterator Greater Than Comparison", "[iterator]", vec_type, cvec_type) {
    TestType vec{1,2,3,4};
    SECTION("iterator") {
        REQUIRE(vec.end() > vec.begin());
    }
    SECTION("const_iterator") {
        REQUIRE(vec.cend() > vec.cbegin());
    }
    SECTION("reverse_iterator") {
        REQUIRE(vec.rend() > vec.rbegin());
    }
    SECTION("const_reverse_iterator") {
        REQUIRE(vec.crend() > vec.crbegin());
    }
}
