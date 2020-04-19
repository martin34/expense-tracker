#pragma once

#include <QtCharts/QtCharts>
#include <QtCore/QDate>

namespace gui {

class Chart : public QChart {
public:
  Chart();
  virtual ~Chart() = default;
  virtual void ZoomFromDateToEnd(const QDate &date);
  virtual void ZoomFromStartToDate(const QDate &date);

protected:
  void ClearChart();
};

} // namespace gui