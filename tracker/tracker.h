#pragma once

#include <iostream>
#include <memory>
#include <tuple>
#include <vector>

#include "core/categories/i_category_filter.h"
#include "core/course.h"
#include "core/stream/utils.h"
#include "core/transactions.h"
#include "parser/rba_parser.h"

namespace finance {

enum class ImportFormat { kN26, kRBA };
class Tracker {
public:
  Tracker() {}

  void LoadFilterConfig(std::istream &config);
  void LoadProjectFrom(std::istream &stream);
  void ImportFrom(std::istream &stream, ImportFormat format);
  void SaveProjectTo(std::ostream &out) const;

  void UpdateCategories();

  Course GenerateTotalCourse() const;
  CoursePerCategory GenerateCoursePerCategory() const;
  std::vector<Transaction>
  GetTransactionsFromDateTo(const Date &begin, const Date &end,
                            Transaction::Category category) const;

  Transactions &GetTransactions() { return transactions_; };

  virtual ~Tracker() = default;

private:
  std::vector<std::unique_ptr<ICategoryFilter>> category_filter_;
  Transactions transactions_;
};

} // namespace finance
