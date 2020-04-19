#include <QtCharts/QtCharts>

#include "core/pivot_table.h"
#include "tracker/tracker.h"

namespace gui {

class PieChart : public QChart {
  Q_OBJECT
public:
  PieChart(const finance::Tracker &tracker);
  void Update(const finance::CoursePerCategory &courses,
              const finance::Date &start, const finance::Date &end);
  void ChangeStartTime(const finance::Date &start);
  void ChangeEndTime(const finance::Date &end);

  virtual ~PieChart() = default;

private slots:
  void OnClickedSlice(const QPieSlice *slice);

private:
  void UpdateView();

private:
  finance::CoursePerCategory courses_{};
  finance::Date start_{};
  finance::Date end_{};
  const finance::Tracker &tracker_;
};
} // namespace gui