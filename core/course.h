#pragma once

#include <string>
#include <vector>

#include "date.h"
#include "utils.h"

namespace finance {

class Course {
public:
  Course() : Course{"date", "amount"} {};
  Course(const std::string &x_label, const std::string &y_label)
      : x_label_{x_label}, y_label_{y_label} {}
  std::string GetXLabel() const { return x_label_; };
  std::string GetYLabel() const { return y_label_; };
  std::vector<Date> GetXValues() const { return x_values_; };
  std::vector<double> GetYValues() const { return y_values_; };
  void AddEntry(const Date &x, double y);
  void SumUpAndAdd(const Date &x, double y);
  using XIterator = std::vector<finance::Date>::const_iterator;
  XIterator XCBegin() const { return x_values_.cbegin(); }
  XIterator XCEnd() const { return x_values_.cend(); }
  using YIterator = std::vector<double>::const_iterator;
  YIterator YCBegin() const { return y_values_.cbegin(); }
  YIterator YCEnd() const { return y_values_.cend(); }

  class CourseIterator : public std::iterator<std::forward_iterator_tag,
                                              CourseIterator, std::size_t> {
  public:
    CourseIterator() {} // Constructor is required for std::partition_point; The
                        // result of this constructor is useless
    CourseIterator(Course::XIterator x_it, Course::YIterator y_it)
        : x_it_{x_it}, y_it_{y_it} {}
    static CourseIterator cbegin(const Course &course) {
      return CourseIterator{course.XCBegin(), course.YCBegin()};
    }
    static CourseIterator cend(const Course &course) {
      return CourseIterator{course.XCEnd(), course.YCEnd()};
    }
    CourseIterator operator++() {
      ++x_it_;
      ++y_it_;
      return *this;
    }
    bool operator==(const CourseIterator &rhs) {
      return (this->x_it_ == rhs.x_it_) && (this->y_it_ == rhs.y_it_);
    }
    bool operator!=(const CourseIterator &rhs) { return !(*this == rhs); }
    std::pair<Date, double> operator*() { return {*x_it_, *y_it_}; }

    class PointerToPair {
    public:
      PointerToPair(Course::XIterator x_it, Course::YIterator y_it)
          : x_it_{x_it}, y_it_{y_it} {}
      std::pair<Date, double> *operator->() {
        current_ = std::make_pair(*x_it_, *y_it_);
        return &current_;
      }

    private:
      Course::XIterator x_it_{};
      Course::YIterator y_it_{};
      std::pair<Date, double> current_{};
    };

    PointerToPair operator->() { return PointerToPair{x_it_, y_it_}; }

  private:
    Course::XIterator x_it_{};
    Course::YIterator y_it_{};
  };

  CourseIterator CBegin() const { return CourseIterator::cbegin(*this); }
  CourseIterator CEnd() const { return CourseIterator::cend(*this); }

private:
  std::string x_label_{};
  std::string y_label_{};
  std::vector<finance::Date> x_values_{};
  std::vector<double> y_values_{};
};

bool operator==(const Course &lhs, const Course &rhs);
bool operator!=(const Course &lhs, const Course &rhs);
std::ostream &operator<<(std::ostream &os, const Course &out);

double Accumulate(Course::CourseIterator it_begin,
                  Course::CourseIterator it_end, const Date &start,
                  const Date &end);
} // namespace finance