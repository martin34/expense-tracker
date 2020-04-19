#include "course.h"
#include <numeric>

namespace finance {
void Course::AddEntry(const Date &x, double y) {
  x_values_.push_back(x);
  y_values_.push_back(y);
}

void Course::SumUpAndAdd(const Date &x, double y) {
  if (!y_values_.empty()) {
    AddEntry(x, y + y_values_.back());
  } else {
    AddEntry(x, y);
  }
}

bool operator==(const Course &lhs, const Course &rhs) {
  auto lhs_y_values = lhs.GetYValues();
  auto rhs_y_values = rhs.GetYValues();
  return (lhs.GetXLabel() == rhs.GetXLabel()) &&
         (lhs.GetYLabel() == rhs.GetYLabel()) &&
         (lhs.GetXValues() == rhs.GetXValues()) &&
         std::equal(lhs_y_values.cbegin(), lhs_y_values.cend(),
                    rhs_y_values.cbegin(), rhs_y_values.cend(),
                    utils::DoubleEqual);
}
bool operator!=(const Course &lhs, const Course &rhs) { return !(lhs == rhs); }
std::ostream &operator<<(std::ostream &os, const Course &out) {
  os << out.GetXLabel() << ";";
  os << out.GetYLabel() << '\n';
  auto x_values = out.GetXValues();
  auto y_values = out.GetYValues();
  auto x_begin = std::cbegin(x_values);
  auto y_begin = std::cbegin(y_values);
  for (; (y_begin != std::cend(y_values)) && (x_begin != std::cend(x_values));
       ++y_begin, ++x_begin) {
    os << *x_begin << ";" << *y_begin << '\n';
  }
  return os;
}
double Accumulate(Course::CourseIterator it_begin,
                  Course::CourseIterator it_end, const Date &start,
                  const Date &end) {
  auto first = std::find_if(
      it_begin, it_end, [&start](const auto &p) { return p.first >= start; });
  if (first == it_end)
    return 0;
  auto last = std::find_if(first, it_end,
                           [&end](const auto &p) { return p.first > end; });

  return std::accumulate(first, last, std::pair<Date, double>{},
                         [](const auto &lhs, const auto &rhs) {
                           return std::make_pair<Date, double>(
                               Date{}, lhs.second + rhs.second);
                         })
      .second;
}
} // namespace finance