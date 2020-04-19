#pragma once

#include "i_category_filter.h"

namespace finance {
class IbanFilter : public ICategoryFilter {
public:
  IbanFilter(const Iban &iban, Transaction::Category category)
      : iban_{iban}, category_{category} {};

  std::optional<Transaction::Category>
  HasCategory(const Transaction &transaction) const override {
    if (!iban_.empty()) {
      if (transaction.partner == iban_)
        return {category_};
    }
    return {};
  }

  ~IbanFilter() = default;

private:
  Iban iban_{};
  Transaction::Category category_{};
};
} // namespace finance
