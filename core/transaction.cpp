#include "transaction.h"
#include "utils.h"

namespace finance {
bool operator==(const Transaction &lhs, const Transaction &rhs) {
  return (lhs.date == rhs.date) && utils::DoubleEqual(lhs.amount, rhs.amount) &&
         (lhs.partner == rhs.partner) &&
         (lhs.partner_name == rhs.partner_name) &&
         (lhs.category == rhs.category) && (lhs.reason == rhs.reason) &&
         (lhs.type == rhs.type);
}
bool operator!=(const Transaction &lhs, const Transaction &rhs) {
  return !(lhs == rhs);
}
std::ostream &operator<<(std::ostream &os, const Transaction &transaction) {
  os << "IBAN: " << transaction.partner << " ";
  os << "PartnerName: " << transaction.partner_name << " ";
  os << "Date: " << transaction.date << " ";
  os << "Amount: " << transaction.amount << " ";
  os << "Category: " << transaction.category << "\n";
  os << "Reason: " << transaction.reason << "\n";
  os << "Type: " << transaction.type << "\n";
  return os;
}

std::ostream &operator<<(std::ostream &os,
                         const Transaction::Category category) {
  os << to_string(category);
  return os;
}
std::string to_string(const Transaction::Category category) {
  switch (category) {
  case Transaction::Unknown:
    return "Unknown";
    break;
  case Transaction::Cash:
    return "Cash";
    break;
  case Transaction::Insurance:
    return "Insurance";
    break;
  case Transaction::Essentials:
    return "Essentials";
    break;
  case Transaction::Sport:
    return "Sport";
    break;
  case Transaction::Rent:
    return "Rent";
    break;
  case Transaction::Sparetime:
    return "Sparetime";
    break;
  case Transaction::Learning:
    return "Learning";
    break;
  case Transaction::Internal:
    return "Internal";
    break;
  case Transaction::Income:
    return "Income";
    break;
  case Transaction::SupportOthers:
    return "SupportOthers";
    break;
  case Transaction::Investment:
    return "Investment";
    break;
  default:
    return "to_string for Transaction::Category not fully implemented";
    break;
  }
}

Transaction::Category ParseTypeFrom(const std::string &word) {
  if (word == "Unknown")
    return Transaction::Unknown;
  if (word == "Cash")
    return Transaction::Cash;
  if (word == "Insurance")
    return Transaction::Insurance;
  if (word == "Essentials")
    return Transaction::Essentials;
  if (word == "Sport")
    return Transaction::Sport;
  if (word == "Sparetime")
    return Transaction::Sparetime;
  if (word == "Learning")
    return Transaction::Learning;
  if (word == "Internal")
    return Transaction::Internal;
  if (word == "Income")
    return Transaction::Income;
  if (word == "SupportOthers")
    return Transaction::SupportOthers;
  if (word == "Rent")
    return Transaction::Rent;
  if (word == "Investment")
    return Transaction::Investment;
  throw std::domain_error("Can't parsecategoryfrom string: " + word);
}
} // namespace finance