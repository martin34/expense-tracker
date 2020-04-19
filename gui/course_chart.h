#pragma once

#include "chart.h"
#include "core/course.h"

namespace gui {
class CourseChart : public Chart {
public:
  CourseChart() : Chart() { setTitle("Accumulated spending"); }

  void Update(const finance::Course &course);

  virtual ~CourseChart() = default;

private:
  void AddAxis(QLineSeries *series);
};

} // namespace gui