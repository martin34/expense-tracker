#pragma once

#include <regex>

#include "i_category_filter.h"

namespace finance {
class ReasonFilter : public ICategoryFilter {
public:
  ReasonFilter(const std::regex &reason, Transaction::Category category)
      : reason_{reason}, category_{category} {};
  std::optional<Transaction::Category>
  HasCategory(const Transaction &transaction) const override {
    if (std::regex_search(transaction.reason, reason_)) {
      return {category_};
    }
    return {};
  }

  ~ReasonFilter() = default;

private:
  std::regex reason_{};
  Transaction::Category category_{};
};
} // namespace finance
