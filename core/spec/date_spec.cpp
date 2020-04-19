
#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "core/date.h"
#include <functional>

namespace {
using namespace ::testing;
using namespace finance;

class DateConstructorFixture : public TestWithParam<std::function<Date()>> {};

INSTANTIATE_TEST_SUITE_P(Constructor, DateConstructorFixture,
                         Values(std::function<Date()>{[]() -> Date {
                                  return Date{1999, 13, 1};
                                }},
                                std::function<Date()>{[]() -> Date {
                                  return Date{1999, 0, 1};
                                }},
                                std::function<Date()>{[]() -> Date {
                                  return Date{1999, 12, 0};
                                }},
                                std::function<Date()>{[]() -> Date {
                                  return Date{0, 12, 1};
                                }},
                                std::function<Date()>{[]() -> Date {
                                  return Date{2021, 2, 29};
                                }},
                                std::function<Date()>{[]() -> Date {
                                  return Date{1999, 12, 32};
                                }}));

TEST_P(DateConstructorFixture, WhenInvalidArgument) {
  EXPECT_THROW(GetParam()(), std::logic_error);
}

TEST(DateComparison, SmallerYear) {
  Date lhs{2009, 10, 10};
  Date rhs{2007, 1, 1};
  EXPECT_THAT(lhs, Gt(rhs));
}
TEST(DateComparison, SmallerYearReverse) {
  Date rhs{2009, 10, 10};
  Date lhs{2007, 1, 1};
  EXPECT_THAT(lhs, Lt(rhs));
}
TEST(DateComparison, SmallerYearNotEqual) {
  Date rhs{2009, 10, 10};
  Date lhs{2007, 1, 1};
  EXPECT_THAT(lhs, Ne(rhs));
}
TEST(DateComparison, SmallerMonth) {
  Date lhs{2009, 1, 10};
  Date rhs{2009, 10, 1};
  EXPECT_THAT(lhs, Lt(rhs));
}
TEST(DateComparison, SmallerMonthReverse) {
  Date rhs{2009, 1, 10};
  Date lhs{2009, 10, 1};
  EXPECT_THAT(lhs, Gt(rhs));
}
TEST(DateComparison, SmallerDate) {
  Date lhs{2009, 1, 1};
  Date rhs{2009, 1, 10};
  EXPECT_THAT(lhs, Lt(rhs));
}
TEST(DateComparison, Ge) {
  Date lhs{2009, 1, 1};
  Date rhs{2009, 1, 1};
  EXPECT_THAT(lhs, Ge(rhs));
}
TEST(DateComparison, Le) {
  Date lhs{2009, 1, 1};
  Date rhs{2009, 1, 1};
  EXPECT_THAT(lhs, Le(rhs));
}
TEST(DateComparison, SmallerWhenSame) {
  Date lhs{2009, 1, 1};
  Date rhs{2009, 1, 1};
  EXPECT_FALSE(lhs < rhs);
  EXPECT_FALSE(rhs < lhs);
}
TEST(DateComparison, SmallerWhenDifferent) {
  Date lhs{2009, 1, 1};
  Date rhs{2009, 2, 1};
  EXPECT_TRUE(lhs < rhs);
  EXPECT_FALSE(rhs < lhs);
}
TEST(DateComparison, SmallerWhenDifferentYearsMonth) {
  Date lhs{2007, 1, 1};
  Date rhs{2009, 2, 1};
  EXPECT_TRUE(lhs < rhs);
  EXPECT_FALSE(rhs < lhs);
}
TEST(DateComparison, FooSmallerWhenDifferentYearsMonth) {
  Date lhs{2007, 1, 10};
  Date rhs{2009, 2, 1};
  EXPECT_TRUE(lhs < rhs);
  EXPECT_FALSE(rhs < lhs);
}
} // namespace