#pragma once

#include "chart.h"
#include "core/pivot_table.h"
#include "tracker/tracker.h"

namespace gui {

class BarChart : public Chart {
  Q_OBJECT
public:
  BarChart(const finance::Tracker &tracker);
  void Update(const finance::PivotTable &pivot_table);

  virtual void ZoomFromDateToEnd(const QDate &date);
  virtual void ZoomFromStartToDate(const QDate &date);

  virtual ~BarChart() = default;

private slots:
  void OnClickedSlice(int index, QBarSet const *const slice);

private:
  void AddAxis(QAbstractSeries *series, QAbstractSeries *income_series,
               const finance::PivotTable &pivot_table);

private:
  const finance::Tracker &tracker_;
  QStringList categories_{};
};

QStackedBarSeries *CreateOutgoingSeries(const finance::PivotTable &pivot_table);
QStringList CreateTicks(const finance::PivotTable &pivot_table);
} // namespace gui
