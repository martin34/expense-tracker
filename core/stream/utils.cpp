#include <regex>
#include <sstream>

#include "utils.h"

namespace finance {

void ValidateCsvHeader(const std::string &line);
Transaction ParseTransactionFrom(const std::string &line);

std::vector<Transaction> ReadTransactionsFromCsv(std::istream &istream) {
  std::string line;
  getline(istream, line);
  ValidateCsvHeader(line);

  std::vector<Transaction> transactions{};
  while (getline(istream, line)) {
    auto t = ParseTransactionFrom(line);
    transactions.push_back(t);
  }
  return transactions;
}
void WriteTransactionsTo(std::vector<Transaction> transactions,
                         std::ostream &ostream) {
  ostream << CsvHeader() << '\n';
  for (const auto &transaction : transactions) {
    ostream << ToCsvLine(transaction) << '\n';
  }
}

std::string CsvHeader() {
  std::stringstream stream{};
  stream << "date" << ';';
  stream << "amount" << ';';
  stream << "partner_name" << ';';
  stream << "partner" << ';';
  stream << "category" << ';';
  stream << "reason" << ';';
  stream << "type";
  return stream.str();
}
void ValidateCsvHeader(const std::string &line) {
  if (line != CsvHeader()) {
    throw std::domain_error("Csv has not the right columns");
  }
}

std::string ToCsvLine(const Transaction &transaction) {
  std::stringstream stream{};
  stream << transaction.date << ';';
  stream << transaction.amount << ';';
  stream << transaction.partner_name << ';';
  stream << transaction.partner << ';';
  stream << transaction.category << ';';
  stream << transaction.reason << ';';
  stream << transaction.type;
  return stream.str();
}
Transaction ParseTransactionFrom(const std::string &line) {
  std::stringstream s{line};
  std::string word{};
  Transaction t{};
  int column_number{};
  while (getline(s, word, ';')) {
    switch (column_number) {
    case 0:
      t.date = ParseDateFrom(word);
      break;
    case 1: {
      std::stringstream ss;
      ss << word;
      ss >> t.amount;
    } break;
    case 2:
      if (word.empty()) {
        throw std::domain_error("Partner name can't be empty");
      }
      t.partner_name = word;
      break;
    case 3:
      t.partner = word;
      break;
    case 4:
      t.category = ParseTypeFrom(word);
      break;
    case 5:
      t.reason = word;
      break;
    case 6:
      t.type = word;
      break;
    default:
      throw std::domain_error("Additional elements in csv");
      break;
    }
    ++column_number;
  }
  return t;
}

} // namespace finance