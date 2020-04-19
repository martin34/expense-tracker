#pragma once

#include <memory>
#include <optional>
#include <vector>

#include "core/transaction.h"

namespace finance {
class ICategoryFilter {
public:
  virtual std::optional<Transaction::Category>
  HasCategory(const Transaction &transaction) const = 0;
  virtual ~ICategoryFilter() = default;
};

Transaction::Category
DetermineCategory(const Transaction &transaction,
                  const std::vector<std::unique_ptr<ICategoryFilter>> &filters);

} // namespace finance
