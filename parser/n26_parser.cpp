#include "n26_parser.h"

#include <algorithm>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>

namespace finance {
bool IsHeaderValid(const std::string &header) {
  const std::string n26_header{
      "\"Datum\",\"Empfänger\",\"Kontonummer\",\"Transaktionstyp\","
      "\"Verwendungszweck\",\"Kategorie\",\"Betrag (EUR)\",\"Betrag "
      "(Fremdwährung)\",\"Fremdwährung\",\"Wechselkurs\""};
  return header == n26_header;
}

std::vector<std::string> ExtractCellsFromLine(const std::string &line);
Date ParseDateFromMinusSeperated(const std::string &line);
std::vector<Transaction> ParseTransactionsFromN26Csv(std::istream &stream) {
  std::string line{};
  getline(stream, line);
  if (!IsHeaderValid(line)) {
    throw std::domain_error(
        "Invalid csv header (n26), can't parse transactions");
  }

  std::vector<Transaction> transactions{};
  while (getline(stream, line)) {
    auto cells = ExtractCellsFromLine(line);
    Transaction t{};
    t.date = ParseDateFromMinusSeperated(cells[0]);
    t.partner_name = cells[1];
    t.partner = Iban{cells[2]};
    t.amount = std::stod(cells[6]);
    t.reason = cells[4];
    t.type = cells[3];
    transactions.push_back(t);
  }
  std::cout << "Parsed " << transactions.size() << " transactions" << '\n';
  return transactions;
}

Date ParseDateFromMinusSeperated(const std::string &line) {

  std::smatch result;
  std::regex date_regex{R"delim((\d{4})-(\d{2})-(\d{2}))delim"};
  auto match = std::regex_search(line, result, date_regex);
  if (match) {
    return Date{std::stoi(result[1]), std::stoi(result[2]),
                std::stoi(result[3])};
  }
  return {};
}

std::vector<std::string> ExtractCellsFromLine(const std::string &line) {
  std::smatch result;
  std::regex pattern{
      R"delim("([^"]*)","([^"]*)","([^"]*)","([^"]*)","([^"]*)","([^"]*)","([^"]*)","([^"]*)","([^"]*)","([^"]*)")delim"};
  std::vector<std::string> cells{};
  if (std::regex_search(line, result, pattern)) {
    int i{1};
    for (; i < 11; ++i) {
      cells.emplace_back(result[i]);
    }
  } else {
    throw std::domain_error("Invaldid csv, not enough columns in line");
  }
  return cells;
}

} // namespace finance