#pragma once

#include <istream>
#include <ostream>
#include <vector>

#include "core/transaction.h"

namespace finance {

void WriteTransactionsTo(std::vector<Transaction> transactions,
                         std::ostream &ostream);

std::string CsvHeader();
std::string ToCsvLine(const Transaction &transaction);
std::vector<Transaction> ReadTransactionsFromCsv(std::istream &istream);

} // namespace finance