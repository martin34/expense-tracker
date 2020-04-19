#include "course_chart.h"

namespace gui {

QLineSeries *CreateSeries(const finance::Course &course);
void CourseChart::Update(const finance::Course &course) {
  ClearChart();
  QLineSeries *series = CreateSeries(course);
  addSeries(series);
  AddAxis(series);
}

void CourseChart::AddAxis(QLineSeries *series) {
  QDateTimeAxis *axis_x = new QDateTimeAxis;
  axis_x->setTickCount(10);
  axis_x->setFormat("MMM yyyy");
  axis_x->setTitleText("Date");
  addAxis(axis_x, Qt::AlignBottom);
  series->attachAxis(axis_x);

  QValueAxis *axis_y = new QValueAxis;
  axis_y->setTitleText("Amount in EUR");
  addAxis(axis_y, Qt::AlignLeft);
  series->attachAxis(axis_y);
}

QLineSeries *CreateSeries(const finance::Course &course) {
  QLineSeries *series = new QLineSeries();
  for (auto first = course.CBegin(); first != course.CEnd(); ++first) {
    auto date = first->first;
    QDateTime moment_in_time;
    moment_in_time.setDate(
        QDate(date.GetYear(), date.GetMonth(), date.GetDay()));
    series->append(moment_in_time.toMSecsSinceEpoch(), first->second);
  }
  return series;
}

} // namespace gui