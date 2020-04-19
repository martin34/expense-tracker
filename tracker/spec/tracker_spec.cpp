#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <sstream>

#include "core/course.h"
#include "core/date.h"
#include "core/spec/test_utils.h"
#include "core/transaction.h"
#include "parser/rba_parser.h"
#include "tracker/tracker.h"

namespace {
using namespace ::testing;
using namespace ::finance;

class TrackerTestFixture : public Test {
protected:
  Tracker tracker_{};
};

TEST_F(TrackerTestFixture, WhenSingleEntryInReport) {
  std::stringstream stream{};
  std::vector<Transaction> transactions;
  Date date{2001, 1, 1};
  double amount{10.0};
  transactions.push_back(AnTransaction()
                             .At(date)
                             .With("some_name")
                             .WithAmount(amount)
                             .WithCategory(Transaction::Cash)
                             .Build());
  WriteTransactionsTo(transactions, stream);
  tracker_.LoadProjectFrom(stream);

  auto total_course = tracker_.GenerateTotalCourse();

  auto expected_total_course = Course{"date", "amount"};
  expected_total_course.AddEntry(date, amount);
  EXPECT_THAT(total_course, Eq(expected_total_course));

  auto course_per_category = tracker_.GenerateCoursePerCategory();
  CoursePerCategory expected_course_per_category{};
  expected_course_per_category[Transaction::Cash] = expected_total_course;
  EXPECT_THAT(course_per_category, Eq(expected_course_per_category));
}

TEST_F(TrackerTestFixture, UpdateFilter) {
  const std::string filer_config{R"(Category;FilterType;Configuration
Essentials;Reason;Miete
Insurance;IBAN;DE67600600000856900000)"};
  std::stringstream filter_config_stream(filer_config);
  tracker_.LoadFilterConfig(filter_config_stream);

  std::stringstream stream{};
  std::vector<Transaction> transactions;
  transactions.push_back(AnTransaction()
                             .At(Date{2001, 1, 1})
                             .With("some_name")
                             .WithAmount(10.0)
                             .Build());
  transactions.push_back(AnTransaction()
                             .At(Date{2002, 1, 1})
                             .With("some_name")
                             .WithAmount(10.0)
                             .WithIban("DE67600600000856900000")
                             .Build());

  WriteTransactionsTo(transactions, stream);
  tracker_.LoadProjectFrom(stream);
  tracker_.UpdateCategories();
  std::stringstream out;
  tracker_.SaveProjectTo(out);
  auto result = ReadTransactionsFromCsv(out);
  EXPECT_THAT(result.at(1).category, Eq(Transaction::Category::Insurance));
}

TEST_F(TrackerTestFixture, GetTransactionsFromTo) {
  std::stringstream stream{};
  std::vector<Transaction> transactions;
  transactions.push_back(AnTransaction()
                             .At(Date{2001, 1, 1})
                             .With("some_name")
                             .WithAmount(10.0)
                             .Build());
  transactions.push_back(AnTransaction()
                             .At(Date{2001, 1, 1})
                             .With("some one else")
                             .WithAmount(10.0)
                             .WithCategory(Transaction::Category::Essentials)
                             .Build());
  transactions.push_back(AnTransaction()
                             .At(Date{2002, 1, 1})
                             .With("some_name")
                             .WithAmount(10.0)
                             .Build());
  WriteTransactionsTo(transactions, stream);
  tracker_.LoadProjectFrom(stream);

  std::vector<Transaction> result = tracker_.GetTransactionsFromDateTo(
      Date{2000, 1, 1}, Date{2001, 2, 1}, Transaction::Category::Essentials);
  ASSERT_THAT(result.size(), Eq(1));
  EXPECT_THAT(result.at(0), Eq(transactions.at(1)));
}
} // namespace
