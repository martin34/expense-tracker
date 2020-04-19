#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <iostream>
#include <sstream>

#include "core/categories/category_filter_factory.h"
#include "core/categories/i_category_filter.h"

namespace {
using namespace ::testing;
using namespace finance;

const std::string filer_config{R"(Category;FilterType;Configuration
Essentials;Reason;Miete
Essentials;PartnerName;lidl
Essentials;Type;relevant type)"};

TEST(Filter, WhenDetermineViaPartnerName) {
  std::stringstream stream{filer_config};
  auto unit = CreateCategoryFilters(stream);

  Transaction transaction{};
  transaction.date = Date{2018, 1, 1};
  transaction.partner_name = "Lidl Vertriebs GmbH &";
  transaction.partner = Iban{""};
  transaction.amount = -100.0;
  transaction.reason = "";

  EXPECT_THAT(DetermineCategory(transaction, unit),
              Eq(Transaction::Essentials));
}
TEST(Filter, WhenDetermineViaTransactionType) {
  std::stringstream stream{filer_config};
  auto unit = CreateCategoryFilters(stream);

  Transaction transaction{};
  transaction.date = Date{2018, 1, 1};
  transaction.partner_name = "irrelevant";
  transaction.partner = Iban{""};
  transaction.amount = -100.0;
  transaction.reason = "";
  transaction.type = "relevant type";

  EXPECT_THAT(DetermineCategory(transaction, unit),
              Eq(Transaction::Essentials));
}
TEST(Filter, WhenEmptyConifg) {
  std::stringstream stream{};
  EXPECT_THROW(CreateCategoryFilters(stream), std::domain_error);
}
} // namespace