#include "transactions.h"

#include <iostream>

namespace finance {
void Transactions::UpdateCategory(
    std::vector<Transaction>::const_iterator const_it,
    Transaction::Category category) {
  if (const_it != std::cend(transactions_)) {
    auto distance = std::distance(transactions_.cbegin(), const_it);
    auto it = std::begin(transactions_);
    std::advance(it, distance);
    std::cout << "Update category";
    if (it->category == Transaction::Category::Unknown) {
      it->category = category;
    } else if (it->category != category) {
      std::cout << "Conflicting categories in transaction: " << *it
                << "'\n' new transaction would be " << category << '\n';
    } else {
      std::cout << "Nothing to change, same as before";
    }
  }
}
Course GenerateTotalCourse(const Transactions &transactions) {
  Course course{"date", "amount"};
  for (const auto &transaction : transactions.GetSortedTransactions()) {
    course.SumUpAndAdd(transaction.date, transaction.amount);
  }
  return course;
}
CoursePerCategory GenerateCoursePerCategory(const Transactions &transactions) {
  std::map<Transaction::Category, Course> all;
  for (const auto &transaction : transactions.GetSortedTransactions()) {
    all[transaction.category].AddEntry(transaction.date, transaction.amount);
  }
  return all;
}
std::vector<Transaction>
Transactions::GetTransactionsFromDateTo(const Date &begin, const Date &end,
                                        Transaction::Category category) const {
  auto first =
      std::find_if(transactions_.cbegin(), transactions_.cend(),
                   [&begin](const auto &p) { return p.date >= begin; });
  if (first == transactions_.cend())
    return {};
  auto last = std::find_if(first, transactions_.cend(),
                           [&end](const auto &p) { return p.date > end; });

  std::vector<Transaction> results;
  std::copy_if(first, last, std::back_inserter(results),
               [category](const auto &t) { return t.category == category; });
  return results;
}
} // namespace finance