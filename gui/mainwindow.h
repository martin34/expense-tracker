#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <string>

#include <QtCharts/QtCharts>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSlider>

#include "bar_chart.h"
#include "course_chart.h"
#include "pie_chart.h"
#include "tracker/tracker.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace gui {
class TransactionTable;
class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  MainWindow(const std::string &project_path);

  ~MainWindow();

private slots:
  void OnOpenProject();
  void OnImport();
  void OnSave();
  void OnViewReport();
  void OnUpdateCategories();

private:
  void SetupMenu();
  void LoadProjectFrom(const std::string &path);
  void ImportFile(const QString &path);

private:
  Ui::MainWindow *ui{nullptr};

  finance::Tracker tracker_{};
  QAction *open_action_{nullptr};
  QAction *import_action_{nullptr};
  QAction *save_action_{nullptr};
  QAction *view_report_action_{nullptr};
  QAction *update_categories_action_{nullptr};
  QMenu *file_menu_{nullptr};
  std::unique_ptr<QWidget> content_{nullptr};
};
#endif // MAINWINDOW_H
}