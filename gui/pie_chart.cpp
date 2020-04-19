#include "pie_chart.h"
#include "color_map.h"
#include "transaction_table_model.h"
#include "transaction_table_view.h"

namespace gui {
PieChart::PieChart(const finance::Tracker &tracker) : tracker_{tracker} {
  legend()->setVisible(false);
}

QPieSeries *CreatePieSeries(const finance::CoursePerCategory &courses,
                            const finance::Date &start,
                            const finance::Date &end) {
  std::map<finance::Transaction::Category, double> partitions;
  for (const auto &p : courses) {
    partitions[p.first] =
        finance::Accumulate(p.second.CBegin(), p.second.CEnd(), start, end);
  }
  QPieSeries *series = new QPieSeries();
  for (const auto &p : partitions) {
    QPieSlice *slice = new QPieSlice(
        QString::fromStdString(finance::to_string(p.first)), p.second);
    slice->setColor(kColorMap.at(p.first));
    slice->setLabelVisible();
    series->append(slice);
  }
  return series;
}
void PieChart::UpdateView() {
  removeAllSeries();
  QPieSeries *series = CreatePieSeries(courses_, start_, end_);
  addSeries(series);
  connect(series, &QPieSeries::doubleClicked, this, &PieChart::OnClickedSlice);
}
void PieChart::Update(const finance::CoursePerCategory &courses,
                      const finance::Date &start, const finance::Date &end) {
  start_ = start;
  end_ = end;
  courses_ = courses;
  UpdateView();
}
void PieChart::ChangeStartTime(const finance::Date &start) {
  start_ = start;
  UpdateView();
}
void PieChart::ChangeEndTime(const finance::Date &end) {
  end_ = end;
  UpdateView();
}

void PieChart::OnClickedSlice(const QPieSlice *slice) {
  qDebug() << "Clicked to slice: " << slice->label();
  TransactionTable *table =
      new TransactionTable(std::move(tracker_.GetTransactionsFromDateTo(
          start_, end_, finance::ParseTypeFrom(slice->label().toStdString()))));
  TransactionDetailsDialog *dialog = new TransactionDetailsDialog(table);
  dialog->show();
  dialog->setAttribute(Qt::WA_DeleteOnClose);
}
} // namespace gui