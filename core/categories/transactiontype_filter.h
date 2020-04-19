#pragma once

#include <regex>

#include "core/categories/i_category_filter.h"
#include "core/transaction.h"

namespace finance {
class TransactionTypeFilter : public ICategoryFilter {
public:
  TransactionTypeFilter(const std::regex &partner_name_pattern,
                        Transaction::Category category)
      : partner_name_pattern_{partner_name_pattern}, category_{category} {};
  virtual std::optional<Transaction::Category>
  HasCategory(const Transaction &transaction) const {
    if (std::regex_search(transaction.type, partner_name_pattern_)) {
      return {category_};
    }
    return {};
  }
  virtual ~TransactionTypeFilter() = default;

private:
  std::regex partner_name_pattern_{};
  Transaction::Category category_{};
};
} // namespace finance