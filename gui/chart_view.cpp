#include "chart_view.h"

namespace gui {
void ChartView::keyPressEvent(QKeyEvent *event) {
  switch (event->key()) {
  case Qt::Key_Y:
    setRubberBand(QChartView::VerticalRubberBand);
    break;
  case Qt::Key_X:
    setRubberBand(QChartView::HorizontalRubberBand);
    break;
  case Qt::Key_Escape:
    chart()->zoomReset();
    break;
  default:
    break;
  }
}
void ChartView::keyReleaseEvent(QKeyEvent *event) {
  switch (event->key()) {
  case Qt::Key_Y:
    setRubberBand(QChartView::RectangleRubberBand);
    break;
  case Qt::Key_X:
    setRubberBand(QChartView::RectangleRubberBand);
    break;
  default:
    break;
  }
}
} // namespace gui