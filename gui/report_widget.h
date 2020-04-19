#pragma once

#include <QtWidgets/QtWidgets>

#include "tracker/tracker.h"

namespace gui {

class PieChart;
class BarChart;
class CourseChart;
class DateEditLayout;
class ReportWidget : public QWidget {
  Q_OBJECT
public:
  ReportWidget(QBoxLayout *parent, const finance::Tracker &tracker);

  virtual ~ReportWidget() = default;

  void SetupChartView();
  void SetupDateChangeWidgets();
  void ViewReport();

private slots:
  void MinDateChanged(const QDate &date);
  void MaxDateChanged(const QDate &date);

private:
  QBoxLayout *parent_{nullptr};

  std::unique_ptr<QTabWidget, std::function<void(QTabWidget *)>> tab_widget_;

  class DateEditLayout {
  public:
    DateEditLayout(QBoxLayout *parent);
    QDateEdit *GetMaxDateEdit() { return &max_date_; }
    QDateEdit *GetMinDateEdit() { return &min_date_; }

  private:
    QBoxLayout *parent_{nullptr};

    std::unique_ptr<QHBoxLayout, std::function<void(QHBoxLayout *)>>
        horizontal_layout_{nullptr};

    QDateEdit min_date_;
    QLabel min_label_{"Min Time: "};
    QDateEdit max_date_;
    QLabel max_label_{"Max Time: "};
  };
  std::unique_ptr<DateEditLayout> date_edit_layout_;

  CourseChart *total_chart_{nullptr};
  PieChart *pie_chart_{nullptr};
  BarChart *bar_chart_{nullptr};

  const finance::Tracker &tracker_;
};

} // namespace gui