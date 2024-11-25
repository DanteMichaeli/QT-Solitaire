#include <QApplication>
#include <QDebug>
#include <QDirIterator>

#include "gui/mainwindow.h"
/**
 * @brief main function
 * @param argc Argument count
 * @param argv Argument vector
 * @return Application exit status
 */

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  MainWindow mainWindow;
  mainWindow.show();

  return app.exec();
}