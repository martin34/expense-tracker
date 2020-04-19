#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "gui/category_string_completer.h"

namespace {
using namespace ::testing;
using Category = finance::Transaction::Category;
using Expected = std::optional<finance::Transaction::Category>;

struct Params {
  Params(const std::string &input, Expected expected)
      : input(input), expected(expected) {}
  std::string input;
  std::optional<finance::Transaction::Category> expected;
};
class CategoryStringCompleterSpec : public testing::TestWithParam<Params> {
public:
  CategoryStringCompleterSpec() {
    for (auto c : GetParam().input) {
      unit_.UserInput(c);
    }
  }
  gui::CategoryStringCompleter unit_;
};
TEST_P(CategoryStringCompleterSpec, OneCharNotSuitable) {
  const auto input = GetParam().input;
  auto result = unit_.Complete();
  ASSERT_THAT(result.has_value(), Eq(GetParam().expected.has_value()));
  if (GetParam().expected.has_value()) {
    EXPECT_THAT(result.value(), Eq(GetParam().expected.value()));
  }
}
INSTANTIATE_TEST_SUITE_P(Foo, CategoryStringCompleterSpec,
                         Values(Params{"", {Category::Unknown}},
                                Params{"Z", {}},
                                Params{"I", {Category::Income}},
                                Params{"Su", {Category::SupportOthers}},
                                Params{"Int", {Category::Internal}},
                                Params{"z", {}},
                                Params{"i", {Category::Income}},
                                Params{"su", {Category::SupportOthers}},
                                Params{"iNt", {Category::Internal}},
                                Params{"\b", {Category::Unknown}},
                                Params{"iNt\b", {Category::Income}},
                                Params{"iNt\b\b", {Category::Income}},
                                Params{"iNt\b\b\b", {Category::Unknown}}));

TEST(CategoryStringCompleterSpec, Reset) {
  gui::CategoryStringCompleter unit;
  for (auto c : "Int") {
    unit.UserInput(c);
  }
  unit.Reset();
  unit.UserInput('E');
  auto result = unit.Complete();
  ASSERT_THAT(result.has_value(), Eq(true));
  EXPECT_THAT(result.value(), Eq(Category::Essentials));
}
} // namespace