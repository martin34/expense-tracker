#include "chart.h"

namespace gui {
Chart::Chart() : QChart() {
  legend()->setAlignment(Qt::AlignBottom);
  setAnimationOptions(QChart::SeriesAnimations);
}

void Chart::ClearChart() {
  removeAllSeries();
  if (axisX()) {
    QAbstractAxis *axis = axisX();
    removeAxis(axis);
    delete axis;
  }
  if (axisY()) {
    QAbstractAxis *axis = axisY();
    removeAxis(axis);
    delete axis;
  }
}

void Chart::ZoomFromDateToEnd(const QDate &date) {
  if (axisX()) {
    axisX()->setMin(date);
  }
}
void Chart::ZoomFromStartToDate(const QDate &date) {
  if (axisX())
    axisX()->setMax(date);
}
} // namespace gui