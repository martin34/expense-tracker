#pragma once

#include "core/transactions.h"
#include <vector>

namespace finance {

Transaction CreateTransaction(
    const Date &date, const double amount, const std::string &partner_name,
    const Transaction::Category category = Transaction::Category::Unknown);

class AnTransaction {
public:
  AnTransaction &At(const Date &date) {
    transaction_.date = date;
    return *this;
  }
  AnTransaction &With(const std::string &partner_name) {
    transaction_.partner_name = partner_name;
    return *this;
  }
  AnTransaction &WithIban(const Iban &iban) {
    transaction_.partner = iban;
    return *this;
  }
  AnTransaction &WithAmount(double amount) {
    transaction_.amount = amount;
    return *this;
  }
  AnTransaction &WithCategory(Transaction::Category category) {
    transaction_.category = category;
    return *this;
  }
  AnTransaction &WithReason(const std::string &reason) {
    transaction_.reason = reason;
    return *this;
  }
  Transaction Build() { return transaction_; }

private:
  Transaction transaction_{};
};

std::vector<Transaction> CreateSampleTransactions();

} // namespace finance