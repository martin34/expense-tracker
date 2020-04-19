#pragma once

#include <QtCharts/QtCharts>

namespace gui {

class ChartView : public QChartView {
public:
  ChartView(QChart *chart) : QChartView(chart) {
    setRubberBand(QChartView::RectangleRubberBand);
  }
  virtual ~ChartView() = default;

protected:
  void keyPressEvent(QKeyEvent *event);
  void keyReleaseEvent(QKeyEvent *event);
};

} // namespace gui