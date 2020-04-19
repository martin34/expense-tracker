#include <fstream>
#include <iostream>
#include <regex>
#include <streambuf>

#include "rba_parser.h"

namespace finance {

std::vector<Transaction> ParseTransactionsFrom(const std::string &content);
std::vector<Transaction> ParseTransactionsFromRbaTxt(std::istream &stream) {
  auto content = std::string((std::istreambuf_iterator<char>(stream)),
                             std::istreambuf_iterator<char>());
  return ParseTransactionsFrom(content);
}

std::string LoadFile(const std::string &file_path);
Transaction ParseFromTransactionString(const std::string &txt_transaction,
                                       const std::int32_t year);
std::int32_t ParseYear(const std::string &content);

std::vector<Transaction>
ParseTransactionsFromFile(const std::string &file_path) {
  std::string content = LoadFile(file_path);
  return ParseTransactionsFrom(content);
}

std::string RemovePreceidingSpaces(std::string input);
std::string RemovePreceidingSpacesFromFistLine(const std::string &input);
std::string RemoveIndentFromParagraph(std::string input);
std::string ParseType(const std::string &input);
double ParseAmount(const std::string &input);
std::string ParseIban(const std::string &input);
std::string ParseReason(const std::string &input);
std::string ParsePartnerName(const std::string &input);
Date ParseDate(const std::string &input, const std::int32_t year);

Transaction ParseFromTransactionString(const std::string &txt_transaction,
                                       const std::int32_t year) {

  auto txt = RemovePreceidingSpaces(txt_transaction);

  Transaction transaction{};
  transaction.type = ParseType(txt);
  transaction.amount = ParseAmount(txt);
  transaction.partner = ParseIban(txt);
  transaction.reason = ParseReason(txt_transaction);
  transaction.partner_name = ParsePartnerName(txt_transaction);
  transaction.date = ParseDate(txt, year);
  return transaction;
}

std::string LoadFile(const std::string &file_path) {

  std::ifstream input_stream{file_path};
  return std::string((std::istreambuf_iterator<char>(input_stream)),
                     std::istreambuf_iterator<char>());
}
std::vector<Transaction> ParseTransactionsFrom(const std::string &content) {
  auto year = ParseYear(content);

  // Find first transaction
  std::smatch result;            // Iterator to part of the search string
  std::string rest_of_content{}; // Keep search string, to ensure that iterator
                                 // is valid after iteration
  std::regex begin_of_transaction_pattern{"([0-3][0-9]\\.[0-1][0-9]\\. ){2}"};
  auto match = std::regex_search(content, result, begin_of_transaction_pattern);
  std::vector<Transaction> transactions{};

  while (match) {
    std::string begin_of_transaction{result[0]};
    rest_of_content = std::string{result.suffix()};
    std::regex end_of_transaction_pattern{
        "([0-3][0-9]\\.[0-1][0-9]\\. |\n){2}"};
    match =
        std::regex_search(rest_of_content, result, end_of_transaction_pattern);
    if (match) {
      auto txt_transaction =
          begin_of_transaction + std::string{result.prefix()};
      transactions.push_back(ParseFromTransactionString(txt_transaction, year));
      // Find next transaction
      match = std::regex_search(rest_of_content, result,
                                begin_of_transaction_pattern);
    }
  }
  return transactions;
}
std::int32_t ParseYear(const std::string &input) {
  std::regex date_pattern{"[0-3][0-9]\\.[0-1][0-9]\\.(20[0-9]{2})"};
  std::smatch result;
  auto match = std::regex_search(input, result, date_pattern);
  if (match) {
    return std::stoi(result[1]);
  }
  throw std::domain_error{"Year not found in file"};
}

std::string RemovePreceidingSpaces(std::string input) {
  auto txt_without_preceiding_spaces =
      RemovePreceidingSpacesFromFistLine(input);
  return RemoveIndentFromParagraph(txt_without_preceiding_spaces);
}
std::string RemovePreceidingSpacesFromFistLine(const std::string &input) {
  bool found_fist_caracter{false};
  auto space_line_start = std::count_if(input.cbegin(), input.cend(),
                                        [&found_fist_caracter](char c) {
                                          if (found_fist_caracter) {
                                            return false;
                                          }
                                          if (c != ' ') {
                                            found_fist_caracter = true;
                                            return false;
                                          }
                                          return true;
                                        });
  std::string txt_without_preceiding_spaces{input};
  std::string::iterator end_spaces{txt_without_preceiding_spaces.begin()};
  std::advance(end_spaces, space_line_start);
  txt_without_preceiding_spaces.erase(txt_without_preceiding_spaces.begin(),
                                      end_spaces);
  return txt_without_preceiding_spaces;
}
std::string RemoveIndentFromParagraph(std::string input) {
  std::regex preceding_spaces{"\n +"};
  std::smatch spaces;
  std::regex_search(input, spaces, preceding_spaces);
  std::regex regex{std::string{spaces[0]}};
  return std::regex_replace(input, regex, "");
}

std::string ParseType(const std::string &input) {
  std::regex begin_of_transaction_pattern{
      "([0-3][0-9]\\.[0-1][0-9]\\. ){2}(\\S.+?) {2,}"};
  std::smatch results;
  auto match = std::regex_search(input, results, begin_of_transaction_pattern);
  if (match) {
    return results[2];
  }
  throw std::domain_error("Parsing of Type failed");
}

double ParseAmount(const std::string &input) {
  std::regex amount{"([0-9,\\.]+,[0-9]{2}) ([S,H])"};
  std::smatch results;
  auto match = std::regex_search(input, results, amount);
  if (match) {
    std::string convertable = results[1];
    std::stringstream ss;
    std::locale de("de_DE.UTF-8");
    ss.imbue(de);
    ss << convertable;
    double amount{};
    ss >> amount;
    if (results[2] == 'S') {
      return amount * -1;
    }
    return amount;
  }
  throw std::domain_error("No value found in transaction string");
}

std::string ParseIban(const std::string &input) {
  std::regex iban{"IBAN: ([a-z,A-Z,0-9]+)"};
  std::smatch results;
  auto match = std::regex_search(input, results, iban);
  if (match) {
    return results[1];
  }
  return {};
}

std::string ParsePartnerName(const std::string &input) {
  std::regex amount{"\n(.*)\n"};
  std::smatch results;
  auto match = std::regex_search(input, results, amount);
  if (match) {
    std::string second_line = results[1];
    return RemovePreceidingSpacesFromFistLine(second_line);
  }
  throw std::domain_error("No partner name found in transaction string");
}

std::string ParseReason(const std::string &input) {
  std::regex amount{"\n(.*)\n"};
  std::smatch results;
  auto match = std::regex_search(input, results, amount);
  if (match) {
    std::string reason = results.suffix();
    reason = RemovePreceidingSpaces(reason);
    reason.erase(std::remove(std::begin(reason), std::end(reason), '\n'),
                 std::end(reason));
    std::regex spaces{" {2}"};
    return std::regex_replace(reason, spaces, "");
  }
  throw std::domain_error("No reason found in transaction string");
}

Date ParseDate(const std::string &input, const std::int32_t year) {
  std::regex date{"([0-3][0-9]).([0-1][0-9])."};
  std::smatch results;
  auto match = std::regex_search(input, results, date);
  if (match) {
    return Date(year, std::stoi(results[2]), std::stoi(results[1]));
  }
  throw std::domain_error("No Date found in transaction string");
}
} // namespace finance