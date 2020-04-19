#pragma once

#include <iterator>
#include <optional>

#include "core/transaction.h"

namespace gui {
class CategoryStringCompleter {
public:
  CategoryStringCompleter();

  void UserInput(char c);
  void Reset() { input_.clear(); }
  std::optional<finance::Transaction::Category> Complete() const;

private:
  struct Option {
    finance::Transaction::Category category;
    std::string name;
    bool suitable;
  };
  std::string input_;
  std::vector<Option> suitable_categories_;
};
} // namespace gui
