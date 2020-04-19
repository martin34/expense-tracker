#include "i_category_filter.h"

namespace finance {

Transaction::Category DetermineCategory(
    const Transaction &transaction,
    const std::vector<std::unique_ptr<ICategoryFilter>> &filters) {
  std::vector<Transaction::Category> categories{};
  for (const auto &filter : filters) {
    auto category = filter->HasCategory(transaction);
    if (category) {
      categories.push_back(category.value());
    }
  }
  if (categories.size() == 0) {
    return transaction.category;
  }
  if (categories.size() != 1) {
    if (!std::equal(categories.cbegin() + 1, categories.cend(),
                    categories.cbegin())) {
      std::stringstream msg{"More than onecategoryfound for transaction: "};
      msg << transaction << "could be of categories: ";
      for (auto type : categories) {
        msg << " " << type;
      }
      throw std::domain_error(msg.str());
    }
  }
  return categories[0];
}

} // namespace finance