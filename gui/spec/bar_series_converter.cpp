#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "gui/bar_chart.h"

namespace {
using namespace ::testing;
using namespace finance;
using namespace gui;

TEST(CreateBarSeries, WhenEmpty) {
  finance::PivotTable pivot_tabel{};
  QStackedBarSeries expected{};
  QStackedBarSeries *actual = gui::CreateOutgoingSeries(pivot_tabel);
  EXPECT_THAT(actual->barSets().count(), Eq(expected.barSets().count()));
  delete actual;
}
TEST(CreateBarSeries, WhenOneCategory) {
  finance::PivotTable pivot_tabel{};
  pivot_tabel.bins =
      std::vector<Date>{Date{2008, 1, 1}, Date{2008, 2, 1}, Date{2008, 3, 1}};
  pivot_tabel.values[finance::Transaction::Essentials] =
      std::vector<double>{1., 2., 3.};
  QStackedBarSeries expected{};
  QBarSet *set0 = new QBarSet("1.1.2008");
  *set0 << 1. << 2. << 3.;
  expected.append(set0);

  QStackedBarSeries *actual = gui::CreateOutgoingSeries(pivot_tabel);
  ASSERT_THAT(actual->barSets().count(), Eq(expected.barSets().count()));
  for (int i{}; i < actual->barSets().count(); ++i) {
    ASSERT_THAT(actual->barSets()[i]->count(),
                Eq(expected.barSets()[i]->count()));
    QBarSet *lhs = actual->barSets()[i];
    QBarSet *rhs = expected.barSets()[i];
    for (int ii{}; ii < actual->barSets()[i]->count(); ++ii) {
      EXPECT_EQ((*lhs)[ii], (*rhs)[ii]);
    }
  }
  delete actual;
}
TEST(CreateAxis, WhenOneCategory) {
  finance::PivotTable pivot_tabel{};
  pivot_tabel.bins =
      std::vector<Date>{Date{2008, 1, 1}, Date{2008, 2, 1}, Date{2008, 3, 1}};
  pivot_tabel.values[finance::Transaction::Essentials] =
      std::vector<double>{1., 2., 3.};

  QStringList ticks{"1.1.2008", "1.2.2008", "1.3.2008"};
  auto actual = CreateTicks(pivot_tabel);

  EXPECT_THAT(actual, Eq(ticks));
}
} // namespace