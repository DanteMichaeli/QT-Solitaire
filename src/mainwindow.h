#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void fullscreen();

    void openSettings();

    void quit();

    void startGame();

    void backToMenu();

private:
    Ui::MainWindow *ui;
    QStackedWidget *stackedWidget; // Main window's pages

    void switchToPage(int pageIndex);
};

#endif // MAINWINDOW_H
