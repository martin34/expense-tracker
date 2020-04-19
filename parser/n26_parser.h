#pragma once

#include <istream>
#include <string>
#include <vector>

#include "core/transaction.h"

namespace finance {

std::vector<Transaction> ParseTransactionsFromN26Csv(std::istream &stream);
bool IsHeaderValid(const std::string &header);

} // namespace finance