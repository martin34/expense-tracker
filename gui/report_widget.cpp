#include "report_widget.h"
#include "bar_chart.h"
#include "chart_view.h"
#include "course_chart.h"
#include "pie_chart.h"
#include "utils.h"

namespace gui {

ReportWidget::ReportWidget(QBoxLayout *parent, const finance::Tracker &tracker)
    : parent_(parent), tracker_(tracker) {
  SetupChartView();
  SetupDateChangeWidgets();
  ViewReport();
}
void ReportWidget::SetupChartView() {

  tab_widget_ = std::unique_ptr<QTabWidget, std::function<void(QTabWidget *)>>(
      new QTabWidget(this), [parent = parent_](QTabWidget *w) {
        parent->removeWidget(w);
        delete w;
      });
  parent_->addWidget(tab_widget_.get());

  total_chart_ = new CourseChart();

  QChartView *total_chart_view = new ChartView(total_chart_);
  total_chart_view->setRenderHint(QPainter::Antialiasing);
  tab_widget_->addTab(total_chart_view, "Total");

  pie_chart_ = new PieChart(tracker_);

  QChartView *pie_chart_view = new QChartView(pie_chart_);
  pie_chart_view->setRenderHint(QPainter::Antialiasing);
  tab_widget_->addTab(pie_chart_view, "Pie Chart");

  bar_chart_ = new BarChart(tracker_);
  QChartView *bar_chart_view = new ChartView(bar_chart_);
  bar_chart_view->setRenderHint(QPainter::Antialiasing);
  tab_widget_->addTab(bar_chart_view, "Monthly Overview");
}
void ReportWidget::SetupDateChangeWidgets() {

  date_edit_layout_ = std::make_unique<DateEditLayout>(parent_);
  connect(date_edit_layout_->GetMinDateEdit(), &QDateEdit::dateChanged, this,
          &ReportWidget::MinDateChanged);

  connect(date_edit_layout_->GetMaxDateEdit(), &QDateEdit::dateChanged, this,
          &ReportWidget::MaxDateChanged);
}
void ReportWidget::MinDateChanged(const QDate &date) {
  total_chart_->ZoomFromDateToEnd(date);
  bar_chart_->ZoomFromDateToEnd(date);
  pie_chart_->ChangeStartTime(FromQDate(date));
}
void ReportWidget::MaxDateChanged(const QDate &date) {
  total_chart_->ZoomFromStartToDate(date);
  bar_chart_->ZoomFromStartToDate(date);
  pie_chart_->ChangeEndTime(FromQDate(date));
}
void ReportWidget::ViewReport() {
  auto total_course = tracker_.GenerateTotalCourse();
  total_chart_->Update(total_course);

  if (!total_course.GetXValues().empty()) {
    auto min_date = total_course.GetXValues().front();
    QDate qmin_date{min_date.GetYear(), min_date.GetMonth(), min_date.GetDay()};
    auto max_date = total_course.GetXValues().back();
    QDate qmax_date{max_date.GetYear(), max_date.GetMonth(), max_date.GetDay()};

    date_edit_layout_->GetMinDateEdit()->setMinimumDate(qmin_date);
    date_edit_layout_->GetMinDateEdit()->setMaximumDate(qmax_date);
    date_edit_layout_->GetMinDateEdit()->setDate(qmin_date);
    date_edit_layout_->GetMaxDateEdit()->setMinimumDate(qmin_date);
    date_edit_layout_->GetMaxDateEdit()->setMaximumDate(qmax_date);
    date_edit_layout_->GetMaxDateEdit()->setDate(qmax_date);
  }

  pie_chart_->Update(tracker_.GenerateCoursePerCategory(),
                     FromQDate(date_edit_layout_->GetMinDateEdit()->date()),
                     FromQDate(date_edit_layout_->GetMaxDateEdit()->date()));
  auto courses = tracker_.GenerateCoursePerCategory();
  if (!total_course.GetXValues().empty()) {
    bar_chart_->Update(finance::GenerateMonthlyAverageSummary(
        courses, total_course.GetXValues().front(),
        total_course.GetXValues().back()));
  }
}

ReportWidget::DateEditLayout::DateEditLayout(QBoxLayout *parent)
    : parent_(parent) {
  horizontal_layout_ =
      std::unique_ptr<QHBoxLayout, std::function<void(QHBoxLayout *)>>(
          new QHBoxLayout(), [parent = parent_](QHBoxLayout *l) {
            parent->removeItem(l);
            delete l;
          });
  parent_->addLayout(horizontal_layout_.get());
  horizontal_layout_->addWidget(&min_label_);
  horizontal_layout_->addWidget(&min_date_);

  horizontal_layout_->addWidget(&max_label_);
  horizontal_layout_->addWidget(&max_date_);
}

} // namespace gui
