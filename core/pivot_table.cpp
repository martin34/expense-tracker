#include "pivot_table.h"

#include <numeric>

namespace finance {

PivotTable GenerateMonthlyAverageSummary(
    const CoursePerCategory &transactions_per_category, const Date &start,
    const Date &end) {
  PivotTable table{};
  table.bins = CreateBinsMonthly(start, end);
  table.values = PartitionToBins(transactions_per_category, table.bins);
  return table;
}
std::vector<Date> CreateBinsMonthly(const Date &start, const Date &end) {
  auto first = start.CreateDateAtEndOfMonth();
  auto last = end.CreateDateAtEndOfMonth();

  std::vector<Date> bins;
  for (Date current{first}; current <= last;
       current = current.CreateDateInNMonth(1)) {
    bins.push_back(current);
  }
  return bins;
}
std::map<Transaction::Category, std::vector<double>>
PartitionToBins(const CoursePerCategory &transactions_per_category,
                const std::vector<Date> &bins) {
  std::map<Transaction::Category, std::vector<double>> partitions{};

  for (const auto &cv : transactions_per_category) {
    auto begin_of_partition = cv.second.CBegin();

    for (const auto &bin_end : bins) {

      auto end_of_partion = std::partition_point(
          begin_of_partition, cv.second.CEnd(),
          [&bin_end](const auto &e) { return e.first <= bin_end; });
      double sum =
          std::accumulate(
              begin_of_partition, end_of_partion, std::make_pair(Date{}, 0.),
              [](const auto &lhs, const auto &rhs) {
                return std::make_pair(Date{}, lhs.second + rhs.second);
              })
              .second;

      partitions[cv.first].push_back(sum);
      begin_of_partition = end_of_partion;
    }
  }
  return partitions;
}
} // namespace finance