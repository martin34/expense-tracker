#include "mainwindow.h"

#include <QtWidgets/QApplication>

#include "cmd/arguments/parser.h"

#include <chrono>
#include <thread>

using namespace std::chrono_literals;

int main(int argc, char *argv[]) {
  cmd::CommandLineArgumentInterface parser{argc, argv};
  QApplication a(argc, argv);
  gui::MainWindow w{parser.GetPathToLoadProjectFrom()};
  w.show();
  a.exec();

  // Give qt more time to clean up memory on shutdown
  std::this_thread::sleep_for(1s);
  return 0;
}
