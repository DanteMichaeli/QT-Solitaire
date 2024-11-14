#include <QApplication>
#include "mainwindow.h"
/**
 * @brief main function
 * @param argc Argument count
 * @param argv Argument vector
 * @return Application exit status
 */
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow mainWindow;  // Create the main window (loaded from .ui file)

    mainWindow.show();      // Show the window

    return app.exec();      // Start the application's event loop
}
