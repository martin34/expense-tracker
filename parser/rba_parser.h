#pragma once

#include "core/transaction.h"
#include <string>
#include <vector>

namespace finance {

Transaction ParseFromTransactionString(const std::string &txt_transaction,
                                       const std::int32_t year);

std::vector<Transaction>
ParseTransactionsFromFile(const std::string &file_path);
std::string LoadFile(const std::string &file_path);
std::vector<Transaction> ParseTransactionsFrom(const std::string &content);
std::vector<Transaction> ParseTransactionsFromRbaTxt(std::istream &stream);
} // namespace finance