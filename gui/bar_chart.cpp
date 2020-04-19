#include "bar_chart.h"

#include <QtCore/QDebug>
#include <QtWidgets/QSizePolicy>

#include <cmath>
#include <vector>

#include "color_map.h"
#include "transaction_table_model.h"
#include "transaction_table_view.h"
#include "utils.h"

namespace gui {

BarChart::BarChart(const finance::Tracker &tracker) : tracker_{tracker} {
  legend()->setVisible(true);
}

QStackedBarSeries *CreateIncomeSeries(const finance::PivotTable &pivot_table);
void BarChart::Update(const finance::PivotTable &pivot_table) {
  ClearChart();
  QAbstractBarSeries *series = CreateOutgoingSeries(pivot_table);
  QAbstractBarSeries *income_series = CreateIncomeSeries(pivot_table);
  addSeries(series);
  connect(series, &QStackedBarSeries::doubleClicked, this,
          &BarChart::OnClickedSlice);
  connect(income_series, &QStackedBarSeries::doubleClicked, this,
          &BarChart::OnClickedSlice);
  addSeries(income_series);
  AddAxis(series, income_series, pivot_table);
}

QString CreateTick(const QDate &date) {
  return QString::fromStdString(
      finance::to_string(FromQDate(date).CreateDateAtEndOfMonth()));
}
void BarChart::ZoomFromDateToEnd(const QDate &date) {
  if (axisX()) {
    axisX()->setMin(CreateTick(date));
  }
}
void BarChart::ZoomFromStartToDate(const QDate &date) {
  if (axisX())
    axisX()->setMax(CreateTick(date));
}

QStackedBarSeries *
CreateOutgoingSeries(const finance::PivotTable &pivot_table) {
  std::vector<QBarSet *> sets;
  for (const auto &p : pivot_table.values) {
    if (!(p.first == finance::Transaction::Income)) {
      sets.emplace_back(
          new QBarSet(QString::fromStdString(finance::to_string(p.first))));
      sets.back()->setColor(kColorMap.at(p.first));
      for (const auto v : p.second) {
        *sets.back() << std::fabs(v);
      }
    }
  }
  QStackedBarSeries *series = new QStackedBarSeries();
  for (const auto s : sets) {
    series->append(s);
  }
  return series;
}
QStackedBarSeries *CreateIncomeSeries(const finance::PivotTable &pivot_table) {
  std::vector<QBarSet *> sets;
  for (const auto &p : pivot_table.values) {
    if (p.first == finance::Transaction::Income) {
      sets.emplace_back(
          new QBarSet(QString::fromStdString(finance::to_string(p.first))));
      for (const auto v : p.second) {
        *sets.back() << std::fabs(v);
        sets.back()->setColor(kColorMap.at(p.first));
      }
    }
  }
  QStackedBarSeries *series = new QStackedBarSeries();
  for (const auto s : sets) {
    series->append(s);
  }
  return series;
}
QStringList CreateTicks(const finance::PivotTable &pivot_table) {
  QStringList categories;
  for (const auto &v : pivot_table.bins) {
    categories << QString::fromStdString(finance::to_string(v));
  }
  return categories;
}
void BarChart::AddAxis(QAbstractSeries *series, QAbstractSeries *income_series,
                       const finance::PivotTable &pivot_table) {
  categories_ = CreateTicks(pivot_table);
  QBarCategoryAxis *axisX = new QBarCategoryAxis();
  axisX->append(categories_);
  addAxis(axisX, Qt::AlignBottom);
  series->attachAxis(axisX);
  income_series->attachAxis(axisX);

  QValueAxis *axisY = new QValueAxis();
  addAxis(axisY, Qt::AlignLeft);
  series->attachAxis(axisY);
  income_series->attachAxis(axisY);
}

void BarChart::OnClickedSlice(int index, QBarSet const *const set) {
  qDebug() << "BarChart double clicked to series with index: " << index << " "
           << set->label();
  finance::Date end{finance::ParseDateFrom(categories_[index].toStdString())};
  finance::Date start{end.GetYear(), end.GetMonth(), 1};
  TransactionTable *table =
      new TransactionTable(std::move(tracker_.GetTransactionsFromDateTo(
          start, end, finance::ParseTypeFrom(set->label().toStdString()))));
  TransactionDetailsDialog *dialog = new TransactionDetailsDialog(table);
  dialog->show();
  dialog->setAttribute(Qt::WA_DeleteOnClose);
}
} // namespace gui