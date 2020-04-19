#include "test_utils.h"

namespace finance {
Transaction CreateTransaction(const Date &date, const double amount,
                              const std::string &partner_name,
                              const Transaction::Category category) {
  Transaction t;
  t.date = date;
  t.amount = amount;
  t.partner_name = partner_name;
  t.category = category;
  return t;
}
std::vector<Transaction> CreateSampleTransactions() {
  std::vector<Transaction> transactions{};
  transactions.push_back(CreateTransaction(Date{2008, 1, 1}, 10.0, "Kitkat",
                                           Transaction::Essentials));
  transactions.push_back(CreateTransaction(Date{2008, 2, 1}, 20.0, "Snickers",
                                           Transaction::Essentials));
  transactions.push_back(CreateTransaction(Date{2008, 3, 1}, 30.0, "Bounty",
                                           Transaction::Sparetime));
  transactions.push_back(CreateTransaction(Date{2008, 4, 1}, 40.0, "Corny",
                                           Transaction::Essentials));
  return transactions;
}
} // namespace finance