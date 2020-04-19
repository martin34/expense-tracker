#include "tracker.h"
#include "core/categories/category_filter_factory.h"
#include "parser/n26_parser.h"

namespace finance {

const static std::locale project_location("en_US.UTF-8");

void Tracker::LoadProjectFrom(std::istream &stream) {
  stream.imbue(project_location);
  auto transactions = ReadTransactionsFromCsv(stream);
  transactions_.Update(std::cbegin(transactions), std::cend(transactions));
}
void Tracker::SaveProjectTo(std::ostream &out) const {
  out.imbue(project_location);
  WriteTransactionsTo(transactions_.GetSortedTransactions(), out);
}

void Tracker::ImportFrom(std::istream &stream, ImportFormat format) {
  std::vector<Transaction> transactions{};
  if (format == ImportFormat::kN26) {
    transactions = ParseTransactionsFromN26Csv(stream);
  } else {
    transactions = ParseTransactionsFromRbaTxt(stream);
  }

  std::cout << "Parsed " + std::to_string(transactions.size())
            << " transactions" << '\n';

  transactions_.Update(std::cbegin(transactions), std::cend(transactions));
}

void Tracker::LoadFilterConfig(std::istream &config) {
  category_filter_ = CreateCategoryFilters(config);
}
void Tracker::UpdateCategories() {
  for (auto it = transactions_.cbegin(); it != transactions_.cend(); ++it) {
    auto category = DetermineCategory(*it, category_filter_);
    transactions_.UpdateCategory(it, category);
  }
  auto count_unknown = std::count_if(
      transactions_.cbegin(), transactions_.cend(),
      [](const Transaction &t) { return t.category == Transaction::Unknown; });
  std::cout << "Elements with unknown category: " << count_unknown << '\n';
}

Course Tracker::GenerateTotalCourse() const {
  return finance::GenerateTotalCourse(transactions_);
}
CoursePerCategory Tracker::GenerateCoursePerCategory() const {
  return finance::GenerateCoursePerCategory(transactions_);
}
std::vector<Transaction>
Tracker::GetTransactionsFromDateTo(const Date &begin, const Date &end,
                                   Transaction::Category category) const {
  return transactions_.GetTransactionsFromDateTo(begin, end, category);
}
} // namespace finance
