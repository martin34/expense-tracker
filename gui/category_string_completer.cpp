#include "category_string_completer.h"

namespace gui {
CategoryStringCompleter::CategoryStringCompleter() {
  std::transform(finance::Categories.cbegin(), finance::Categories.cend(),
                 std::back_inserter(suitable_categories_),
                 [](const finance::Transaction::Category category) {
                   auto tmp = finance::to_string(category);
                   std::transform(
                       tmp.begin(), tmp.end(), tmp.begin(),
                       [](unsigned char c) { return std::tolower(c); });
                   return Option{category, tmp, true};
                 });
}

void CategoryStringCompleter::UserInput(char c) {
  if (c == '\b') {
    if (!input_.empty())
      input_.pop_back();
  } else {
    input_.push_back(c);
  }
}

std::optional<finance::Transaction::Category>
CategoryStringCompleter::Complete() const {
  std::vector<Option> suitable_categories = suitable_categories_;
  std::size_t currend_position{};
  if (input_.empty())
    return {finance::Transaction::Unknown};

  for (auto c : input_) {
    for (auto &category : suitable_categories) {
      if (currend_position < category.name.size()) {
        if (category.name[currend_position] != std::tolower(c)) {
          category.suitable = false;
        }
      } else {
        category.suitable = false;
      }
    }
    ++currend_position;
  }
  for (auto &category : suitable_categories) {
    if (category.suitable)
      return {category.category};
  }
  return {};
}

} // namespace gui