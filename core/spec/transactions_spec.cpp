#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "core/transaction.h"
#include "core/transactions.h"
#include "test_utils.h"

namespace {
using namespace ::testing;
using namespace finance;

TEST(TransactionsSpec, WhenEmpty_UpdateWithEmpty_ThenEmpty) {
  Transactions transactions{};
  std::vector<Transaction> more_transactions;
  transactions.Update(more_transactions.cbegin(), more_transactions.cend());

  auto output = transactions.GetSortedTransactions();
  EXPECT_THAT(output.empty(), Eq(true));
}
TEST(TransactionsSpec,
     WhenEmpty_UpdateWithTransactions_ThenExpectTransactionsSortedByDate) {
  Transactions transactions{};
  std::vector<Transaction> more_transactions;
  more_transactions.push_back(
      AnTransaction().With("a").At(Date{2018, 1, 1}).Build());
  more_transactions.push_back(
      AnTransaction().With("z").At(Date{2017, 1, 1}).Build());

  transactions.Update(more_transactions.cbegin(), more_transactions.cend());

  auto output = transactions.GetSortedTransactions();
  std::vector<Transaction> expected{more_transactions[1], more_transactions[0]};
  EXPECT_THAT(output, ContainerEq(expected));
}
TEST(
    TransactionsSpec,
    WhenEmpty_UpdateWithTransactionsWithEqualDate_ThenExpectTransactionsSortedPartnerName) {
  Transactions transactions{};
  std::vector<Transaction> more_transactions;
  more_transactions.push_back(
      AnTransaction().With("z").At(Date{2018, 1, 1}).Build());
  more_transactions.push_back(
      AnTransaction().With("a").At(Date{2018, 1, 1}).Build());
  transactions.Update(more_transactions.cbegin(), more_transactions.cend());

  auto output = transactions.GetSortedTransactions();
  std::vector<Transaction> expected{more_transactions[1], more_transactions[0]};
  EXPECT_THAT(output, ContainerEq(expected));
}
TEST(
    TransactionsSpec,
    WhenEmpty_UpdateWithTransactionsWithEqualDateAndPartnerName_ThenExpectTransactionsSortedAmount) {
  Transactions transactions{};
  std::vector<Transaction> more_transactions;
  more_transactions.push_back(
      AnTransaction().With("a").At(Date{2018, 1, 1}).WithAmount(2.).Build());
  more_transactions.push_back(
      AnTransaction().With("a").At(Date{2018, 1, 1}).WithAmount(1.).Build());
  transactions.Update(more_transactions.cbegin(), more_transactions.cend());

  auto output = transactions.GetSortedTransactions();
  // std::vector<Transaction> expected{t2, t1};
  std::vector<Transaction> expected{more_transactions[1], more_transactions[0]};
  EXPECT_THAT(output, ContainerEq(expected));
}
TEST(
    TransactionsSpec,
    WhenEmpty_UpdateWithDublicatedTransactions_ThenExpectTransactionsWithoutDublicated) {
  Transactions transactions{};
  std::vector<Transaction> more_transactions;
  more_transactions.push_back(
      AnTransaction().With("a").At(Date{2018, 1, 1}).WithAmount(2.).Build());
  more_transactions.push_back(
      AnTransaction().With("a").At(Date{2017, 1, 1}).WithAmount(1.).Build());
  transactions.Update(more_transactions.cbegin(), more_transactions.cend());
  transactions.Update(more_transactions.cbegin(), more_transactions.cend());

  auto output = transactions.GetSortedTransactions();
  std::vector<Transaction> expected{more_transactions[1], more_transactions[0]};
  EXPECT_THAT(output, ContainerEq(expected));
}
TEST(
    TransactionsSpec,
    WhenEmpty_UpdateWithDublicatedTransactionsWithDifferentCategory_ThenExpectTransactionsWithoutDublicated) {
  Transactions transactions{};
  std::vector<Transaction> more_transactions;
  auto t1 = AnTransaction().With("a").At(Date{2018, 1, 1}).WithAmount(2.);
  auto t2 = AnTransaction().With("a").At(Date{2017, 1, 1}).WithAmount(1.);
  more_transactions.push_back(t1.Build());
  more_transactions.push_back(t2.Build());
  transactions.Update(more_transactions.cbegin(), more_transactions.cend());
  std::vector<Transaction> more_new_transactions;
  more_new_transactions.push_back(
      t1.WithCategory(Transaction::Essentials).Build());
  more_new_transactions.push_back(
      t2.WithCategory(Transaction::Essentials).Build());
  transactions.Update(more_new_transactions.cbegin(),
                      more_new_transactions.cend());

  auto output = transactions.GetSortedTransactions();
  std::vector<Transaction> expected{more_transactions[1], more_transactions[0]};
  EXPECT_THAT(output, ContainerEq(expected));
}
TEST(TransactionsSpec, UpdateCategoryOfTransaction) {
  Transactions transactions{};
  std::vector<Transaction> more_transactions;
  more_transactions.push_back(AnTransaction()
                                  .With("a")
                                  .At(Date{2018, 1, 1})
                                  .WithAmount(2.)
                                  .WithCategory(Transaction::Unknown)
                                  .Build());
  auto t2 = AnTransaction().With("a").At(Date{2019, 1, 1}).WithAmount(1.);
  more_transactions.push_back(t2.WithCategory(Transaction::Unknown).Build());

  transactions.Update(more_transactions.cbegin(), more_transactions.cend());

  auto match = std::find_if(transactions.cbegin(), transactions.cend(),
                            [&t2 = more_transactions[1]](const Transaction &t) {
                              return t.date == t2.date;
                            });
  transactions.UpdateCategory(match, Transaction::Category::Cash);

  auto new_transactions = transactions.GetSortedTransactions();
  EXPECT_THAT(new_transactions.at(0), Eq(more_transactions[0]));
  EXPECT_THAT(new_transactions.at(1),
              Eq(t2.WithCategory(Transaction::Cash).Build()));
}
TEST(TransactionsSpec, UpdateCategoryEmptyTransactions) {
  Transactions transactions{};
  transactions.UpdateCategory(transactions.cend(), Transaction::Category::Cash);
}
TEST(GenerteReport, WhenEmptyTransactions) {
  Transactions transactions{};
  auto total_course = GenerateTotalCourse(transactions);
  Course expected_total_course{"date", "amount"};
  EXPECT_THAT(total_course, Eq(expected_total_course));

  auto course_per_category = GenerateCoursePerCategory(transactions);
  CoursePerCategory expected_course_per_category;
  EXPECT_THAT(course_per_category, Eq(expected_course_per_category));
}
TEST(GenerteReport, WhenTwoTransactionsWithTwoCategories) {
  Transactions transactions{};
  std::vector<Transaction> transactions_{};
  transactions_.push_back(CreateTransaction(Date{2008, 1, 1}, 10.0, "Kitkat",
                                            Transaction::Sparetime));
  transactions_.push_back(CreateTransaction(Date{2008, 2, 1}, 20.0, "Snickers",
                                            Transaction::Essentials));
  transactions.Update(transactions_.cbegin(), transactions_.cend());

  auto total_course = GenerateTotalCourse(transactions);
  Course expected_total_course{"date", "amount"};
  expected_total_course.AddEntry(transactions_[0].date,
                                 transactions_[0].amount);
  expected_total_course.AddEntry(
      transactions_[1].date, transactions_[0].amount + transactions_[1].amount);
  EXPECT_THAT(total_course, Eq(expected_total_course));

  auto course_per_category = GenerateCoursePerCategory(transactions);
  Course sparetime{"date", "amount"};
  sparetime.AddEntry(transactions_[0].date, transactions_[0].amount);
  Course essentials{"date", "amount"};
  essentials.AddEntry(transactions_[1].date, transactions_[1].amount);

  CoursePerCategory expected_course_per_category;
  expected_course_per_category[transactions_[0].category] = sparetime;
  expected_course_per_category[transactions_[1].category] = essentials;
  EXPECT_THAT(course_per_category, Eq(expected_course_per_category));
}
TEST(GenerteReport, WhenFourTransactionsWithTwoCategories) {
  Transactions transactions{};
  auto transactions_ = CreateSampleTransactions();
  transactions.Update(transactions_.cbegin(), transactions_.cend());

  auto total_course = GenerateTotalCourse(transactions);
  Course expected_total_course{"date", "amount"};
  expected_total_course.AddEntry(transactions_[0].date,
                                 transactions_[0].amount);
  expected_total_course.AddEntry(
      transactions_[1].date, transactions_[0].amount + transactions_[1].amount);
  expected_total_course.AddEntry(
      transactions_[2].date, transactions_[0].amount + transactions_[1].amount +
                                 transactions_[2].amount);
  expected_total_course.AddEntry(
      transactions_[3].date, transactions_[0].amount + transactions_[1].amount +
                                 transactions_[2].amount +
                                 transactions_[3].amount);
  EXPECT_THAT(total_course, Eq(expected_total_course));

  auto course_per_category = GenerateCoursePerCategory(transactions);
  CoursePerCategory expected_course_per_category;
  Course sparetime{"date", "amount"};
  sparetime.AddEntry(transactions_[2].date, transactions_[2].amount);
  expected_course_per_category[transactions_[2].category] = sparetime;
  Course essentials{"date", "amount"};
  essentials.AddEntry(transactions_[0].date, transactions_[0].amount);
  essentials.AddEntry(transactions_[1].date, transactions_[1].amount);
  essentials.AddEntry(transactions_[3].date, transactions_[3].amount);
  expected_course_per_category[transactions_[0].category] = essentials;
  EXPECT_THAT(course_per_category, Eq(course_per_category));
}
} // namespace