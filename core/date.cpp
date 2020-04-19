#include "date.h"
#include <boost/date_time/gregorian/greg_duration_types.hpp>
#include <regex>

namespace finance {
bool Date::operator==(const Date &rhs) const { return date_ == rhs.date_; }
bool operator!=(const Date &lhs, const Date &rhs) { return !(lhs == rhs); }

std::ostream &operator<<(std::ostream &os, const Date &date) {
  os << to_string(date);
  return os;
}
std::string to_string(const Date &date) {
  return std::string{std::to_string(date.GetDay()) + "." +
                     std::to_string(date.GetMonth()) + "." +
                     std::to_string(date.GetYear())};
}

Date ParseDateFrom(const std::string &word) {
  std::regex date_pattern{"([0-9]{1,2}).([0-9]{1,2}).([0-9]{1,4})"};
  std::smatch results;
  auto match = std::regex_search(word, results, date_pattern);
  if (match) {
    return Date(std::stoi(results[3]), std::stoi(results[2]),
                std::stoi(results[1]));
  }
  throw std::domain_error{"Can't parse Date from: " + word};
}
bool Date::operator<(const Date &rhs) const { return date_ < rhs.date_; }
bool operator>(const Date &lhs, const Date &rhs) { return rhs < lhs; }
bool operator>=(const Date &lhs, const Date &rhs) {
  return (lhs > rhs) || (rhs == lhs);
}
bool operator<=(const Date &lhs, const Date &rhs) {
  return (lhs < rhs) || (rhs == lhs);
}

Date Date::CreateDateInNMonth(std::int32_t n) const {
  boost::gregorian::months m(n);
  return Date{date_ + m};
}

Date Date::CreateDateAtEndOfMonth() const { return Date{date_.end_of_month()}; }
} // namespace finance