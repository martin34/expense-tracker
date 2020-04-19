#include <fstream>
#include <memory>

#include "category_change_widget.h"
#include "gui/ui_mainwindow.h"
#include "mainwindow.h"
#include "report_widget.h"
#include "utils.h"

#include <QtCharts/QChartView>
#include <QtCore/QAbstractTableModel>
#include <QtCore/QtDebug>
#include <QtWidgets/QTableView>

namespace gui {
MainWindow::MainWindow(const std::string &project_path) : MainWindow() {
  if (!project_path.empty()) {
    LoadProjectFrom(project_path);
    OnViewReport();
  }
}
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  SetupMenu();
}
void MainWindow::SetupMenu() {
  QMenuBar *bar = menuBar();
  file_menu_ = bar->addMenu(tr("&File"));

  open_action_ = new QAction{"&Open Project"};
  connect(open_action_, SIGNAL(triggered()), this, SLOT(OnOpenProject()));
  file_menu_->addAction(open_action_);

  import_action_ = new QAction{"&Import"};
  connect(import_action_, SIGNAL(triggered()), this, SLOT(OnImport()));
  file_menu_->addAction(import_action_);

  save_action_ = new QAction{"&Save Project"};
  connect(save_action_, SIGNAL(triggered()), this, SLOT(OnSave()));
  file_menu_->addAction(save_action_);

  view_report_action_ = new QAction{"&View report"};
  connect(view_report_action_, SIGNAL(triggered()), this, SLOT(OnViewReport()));
  file_menu_->addAction(view_report_action_);

  update_categories_action_ = new QAction{"&Update categories"};
  connect(update_categories_action_, SIGNAL(triggered()), this,
          SLOT(OnUpdateCategories()));
  file_menu_->addAction(update_categories_action_);
}
void MainWindow::OnOpenProject() {
  QString file_name = QFileDialog::getOpenFileName(
      this, tr("Open existing project"), "/home/martin/repo/finance",
      tr("Project Files (*.csv)"));

  if (file_name.isEmpty())
    return;

  LoadProjectFrom(file_name.toStdString());
}
void MainWindow::LoadProjectFrom(const std::string &path) {
  if (!path.empty()) {
    std::ifstream stream{path};
    tracker_.LoadProjectFrom(stream);
  }
}
void MainWindow::ImportFile(const QString &file_name) {
  if (file_name.isEmpty())
    return;
  auto path = file_name.toStdString();
  if (path.size() < 4)
    throw std::domain_error("File path not valid");
  std::string::const_iterator file_ending_it{path.cend()};
  std::advance(file_ending_it, -4);
  std::string file_ending{file_ending_it, path.cend()};
  finance::ImportFormat format{};
  if (file_ending == ".txt") {
    format = finance::ImportFormat::kRBA;
  } else if (file_ending == ".csv") {
    format = finance::ImportFormat::kN26;
  } else {
    throw std::domain_error("File ending not valid. It needs to be .txt for "
                            "the Raiffeisenbank or .csv for N26.");
  }
  std::ifstream stream{path};
  tracker_.ImportFrom(stream, format);
}
void MainWindow::OnImport() {
  QStringList file_names = QFileDialog::getOpenFileNames(
      this, tr("Import more data"), "/home/martin/repo/finance/parser/data/txt",
      tr("Project Files (*.csv *.txt)"));

  for (QStringList::const_iterator begin = file_names.cbegin();
       begin != file_names.cend(); ++begin) {
    ImportFile(*begin);
  }
  std::ifstream filter_config_stream("tracker/.category_config");
  tracker_.LoadFilterConfig(filter_config_stream);
  tracker_.UpdateCategories();
}
void MainWindow::OnSave() {
  QString file_name = QFileDialog::getSaveFileName(
      this, tr("Import more data"), "/home/martin/repo/finance/new_project.csv",
      tr("Project Files (*.csv)"));

  if (file_name.isEmpty())
    return;
  std::ofstream out{file_name.toStdString()};
  tracker_.SaveProjectTo(out);
}
void MainWindow::OnViewReport() {
  content_ =
      std::unique_ptr<QWidget>(new ReportWidget{ui->verticalLayout, tracker_});
}

void MainWindow::OnUpdateCategories() {
  content_ = std::unique_ptr<QWidget>(
      new CategoryChangeWidget{ui->verticalLayout, tracker_});
}

MainWindow::~MainWindow() {
  delete open_action_;
  delete import_action_;
  delete save_action_;
  delete update_categories_action_;
  delete view_report_action_;
  delete file_menu_;
  delete ui;
}

} // namespace gui
