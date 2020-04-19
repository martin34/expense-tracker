#pragma once

#include <ostream>
#include <sstream>
#include <string>

#include "date.h"

namespace finance {

using Iban = std::string;

struct Transaction {
  enum Category {
    Cash,
    Essentials,
    Income,
    Insurance,
    Internal,
    Investment,
    Learning,
    Rent,
    Sparetime,
    Sport,
    SupportOthers,
    Unknown
  } category{Unknown};
  Iban partner;
  std::string partner_name;
  Date date;
  double amount;
  std::string reason;
  std::string type;
};

constexpr auto Categories =
    std::array<Transaction::Category, Transaction::Category::Unknown + 1>{
        Transaction::Cash,          Transaction::Essentials,
        Transaction::Income,        Transaction::Insurance,
        Transaction::Internal,      Transaction::Investment,
        Transaction::Learning,      Transaction::Rent,
        Transaction::Sparetime,     Transaction::Sport,
        Transaction::SupportOthers, Transaction::Unknown};
bool operator==(const Transaction &lhs, const Transaction &rhs);
bool operator!=(const Transaction &lhs, const Transaction &rhs);

std::ostream &operator<<(std::ostream &os,
                         const Transaction::Category category);

std::ostream &operator<<(std::ostream &os, const Transaction &transaction);
std::string to_string(const Transaction::Category category);
Transaction::Category ParseTypeFrom(const std::string &word);

} // namespace finance