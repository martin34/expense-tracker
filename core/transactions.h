#pragma once

#include <algorithm>
#include <iterator>
#include <vector>

#include "pivot_table.h"
#include "transaction.h"

namespace finance {
class Transactions {
public:
  using const_iterator = std::vector<Transaction>::const_iterator;
  template <class InputIt> void Update(InputIt first, InputIt end);
  std::vector<Transaction> GetSortedTransactions() const {
    return transactions_;
  }
  std::vector<Transaction>
  GetTransactionsFromDateTo(const Date &begin, const Date &end,
                            Transaction::Category category) const;

  const_iterator cend() const { return transactions_.cend(); }
  const_iterator cbegin() const { return transactions_.cbegin(); }
  Transaction back() const {
    if (transactions_.empty())
      throw std::logic_error("No elements available, can't return back");
    return transactions_.back();
  }
  Transaction front() const {
    if (transactions_.empty())
      throw std::logic_error("No elements available, can't return front");
    return transactions_.front();
  }
  bool empty() const { return transactions_.empty(); }
  void UpdateCategory(std::vector<Transaction>::const_iterator const_it,
                      Transaction::Category category);

private:
  std::vector<Transaction> transactions_{};
};

template <class InputIt> void Transactions::Update(InputIt first, InputIt end) {
  for (; first != end; ++first) {
    Transaction tmp{*first};
    if (std::find_if(std::cbegin(transactions_), std::cend(transactions_),
                     [&tmp](const auto &v) {
                       return (tmp.date == v.date) &&
                              utils::DoubleEqual(tmp.amount, v.amount) &&
                              (tmp.partner == v.partner) &&
                              (tmp.partner_name == v.partner_name) &&
                              (tmp.reason == v.reason) && (tmp.type == v.type);
                     }) == std::cend(transactions_)) {
      transactions_.push_back(tmp);
    }
  }
  std::sort(transactions_.begin(), transactions_.end(),
            [](const auto &lhs, const auto &rhs) {
              if ((lhs.date == rhs.date) &&
                  (lhs.partner_name == rhs.partner_name))
                return lhs.amount < rhs.amount;
              if (lhs.date == rhs.date)
                return lhs.partner_name < rhs.partner_name;

              return lhs.date < rhs.date;
            });
}

Course GenerateTotalCourse(const Transactions &transactions);

CoursePerCategory GenerateCoursePerCategory(const Transactions &transactions);
} // namespace finance