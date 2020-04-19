#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "core/pivot_table.h"
#include "core/transactions.h"
#include "test_utils.h"

namespace {
using namespace ::testing;
using namespace finance;

PivotTable
CreateMontlyPivotTableForAllTransactions(const Transactions &transactions) {
  return GenerateMonthlyAverageSummary(GenerateCoursePerCategory(transactions),
                                       transactions.front().date,
                                       transactions.back().date);
}

TEST(GenerateReport, PivotTabelWhenOneCategory) {
  Transactions transactions{};
  std::vector<Transaction> t;
  t.push_back(
      CreateTransaction(Date{2020, 1, 1}, 10., "foo", Transaction::Cash));
  t.push_back(
      CreateTransaction(Date{2020, 1, 3}, 30., "bar", Transaction::Cash));
  t.push_back(CreateTransaction(Date{2020, 2, 2}, 20., "second bar",
                                Transaction::Cash));
  t.push_back(
      CreateTransaction(Date{2020, 2, 4}, 60., "third bar", Transaction::Cash));
  transactions.Update(t.cbegin(), t.cend());

  auto report = CreateMontlyPivotTableForAllTransactions(transactions);

  PivotTable table;
  table.bins.push_back(Date{2020, 1, 31});
  table.bins.push_back(Date{2020, 2, 29});
  table.values[Transaction::Cash] = std::vector<double>{40., 80};
  EXPECT_THAT(report.bins, ContainerEq(table.bins));
  EXPECT_THAT(report.values, ContainerEq(table.values));
}

TEST(GenerateReport, PivotTabelWhenMultipleCategories) {
  Transactions transactions{};
  std::vector<Transaction> t;
  t.push_back(
      CreateTransaction(Date{2020, 1, 1}, 10., "foo", Transaction::Cash));
  t.push_back(
      CreateTransaction(Date{2020, 1, 3}, 30., "bar", Transaction::Essentials));
  t.push_back(CreateTransaction(Date{2020, 2, 2}, 20., "second bar",
                                Transaction::Cash));
  t.push_back(CreateTransaction(Date{2020, 2, 4}, 60., "third bar",
                                Transaction::Essentials));
  t.push_back(CreateTransaction(Date{2020, 3, 5}, 70., "third bar",
                                Transaction::Sparetime));
  transactions.Update(t.cbegin(), t.cend());

  auto report = CreateMontlyPivotTableForAllTransactions(transactions);

  PivotTable table;
  table.bins.push_back(Date{2020, 1, 31});
  table.bins.push_back(Date{2020, 2, 29});
  table.bins.push_back(Date{2020, 3, 31});
  table.values[Transaction::Cash] = std::vector<double>{10., 20, 0.};
  table.values[Transaction::Essentials] = std::vector<double>{30., 60, 0.};
  table.values[Transaction::Sparetime] = std::vector<double>{0., 0., 70};
  EXPECT_THAT(report.bins, ContainerEq(table.bins));
  EXPECT_THAT(report.values, ContainerEq(table.values));
}
} // namespace