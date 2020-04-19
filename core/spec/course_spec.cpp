#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "core/course.h"

namespace {
using namespace ::testing;
using namespace ::finance;

TEST(CourseSpec, Equal) {
  Course lhs{"x", "y"};
  lhs.AddEntry(Date{2018, 11, 1}, 10.00);
  Course rhs{"x", "y"};
  rhs.AddEntry(Date{2018, 11, 1}, 10.000000001);
  EXPECT_EQ(lhs, rhs);
}
TEST(CourseSpec, NotEqual) {
  Course lhs{"x", "y"};
  lhs.AddEntry(Date{2018, 11, 1}, 10.00);
  Course rhs{"x", "y"};
  rhs.AddEntry(Date{2018, 11, 1}, 9.00);
  EXPECT_NE(lhs, rhs);
}
TEST(CourseSpec, NotEqualDifferentSizes) {
  Course lhs{"x", "y"};
  lhs.AddEntry(Date{2018, 11, 1}, 10.00);
  Course rhs{"x", "y"};
  EXPECT_NE(lhs, rhs);
}
TEST(CourseSpec, Iterator) {
  Course course{"foo", "bar"};
  course.AddEntry(Date{2018, 11, 1}, 10.00);
  course.AddEntry(Date{2018, 11, 2}, 20.00);
  course.AddEntry(Date{2018, 11, 3}, 30.00);
  Course expected{"foo", "bar"};
  for (auto it = course.CBegin(); it != course.CEnd(); ++it) {
    expected.AddEntry(it->first, it->second);
  }
  EXPECT_THAT(expected, Eq(course));
}
TEST(CourseSpec, IteratorIncrementAndReuse) {
  Course course{"foo", "bar"};
  course.AddEntry(Date{2018, 11, 1}, 10.00);
  course.AddEntry(Date{2018, 11, 2}, 20.00);
  course.AddEntry(Date{2018, 11, 3}, 30.00);
  auto first = course.CBegin();
  auto second = ++first;
  auto third = ++second;
  EXPECT_THAT(*third, Eq(std::make_pair(Date{2018, 11, 3}, 30.00)));
}
TEST(CourseSpec, AccumulateFromTo) {
  Course course;
  course.AddEntry(Date{2018, 11, 1}, 10.00);
  Date start{2018, 11, 2};
  double v1{20.00};
  double v2{30.00};
  double v3{11.00};
  double v4{22.00};
  double v5{33.00};
  course.AddEntry(start, v1);
  course.AddEntry(Date{2018, 11, 3}, v2);
  course.AddEntry(Date{2018, 12, 3}, v3);
  course.AddEntry(Date{2018, 12, 4}, v4);
  Date end{2018, 12, 5};
  course.AddEntry(end, v5);
  course.AddEntry(Date{2019, 11, 5}, 33.00);
  course.AddEntry(Date{2019, 11, 6}, 100.00);
  Course actual;
  double sum = Accumulate(course.CBegin(), course.CEnd(), start, end);
  EXPECT_THAT(sum, Eq(v1 + v2 + v3 + v4 + v5));
}
} // namespace