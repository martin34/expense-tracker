#pragma once

#include <cstdint>
#include <ostream>
#include <stdexcept>
#include <string>

#include <boost/date_time/gregorian/greg_date.hpp>

namespace finance {

class Date {
public:
  Date() : date_{boost::gregorian::special_values::not_a_date_time} {}
  Date(boost::gregorian::date date) : date_(date) {}
  Date(std::int32_t year, std::int32_t month, std::int32_t day)
      : date_{static_cast<std::uint16_t>(year),
              static_cast<std::uint16_t>(month),
              static_cast<std::uint16_t>(day)} {}
  std::int32_t GetYear() const { return date_.year(); }
  std::int32_t GetMonth() const { return date_.month(); }
  std::int32_t GetDay() const { return date_.day(); }

  bool operator<(const Date &rhs) const;
  bool operator==(const Date &rhs) const;
  Date CreateDateInNMonth(std::int32_t n) const;
  Date CreateDateAtEndOfMonth() const;

private:
  boost::gregorian::date date_;
};
// Replace with spaceship
bool operator!=(const Date &lhs, const Date &rhs);
bool operator>(const Date &lhs, const Date &rhs);
bool operator<=(const Date &lhs, const Date &rhs);
bool operator>=(const Date &lhs, const Date &rhs);

std::ostream &operator<<(std::ostream &os, const Date &date);
std::string to_string(const Date &date);
Date ParseDateFrom(const std::string &word);

} // namespace finance