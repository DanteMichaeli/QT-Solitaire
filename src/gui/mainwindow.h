#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QResizeEvent>
#include <QStackedWidget>
#include "settings.hpp"
#include "gameView.hpp"

namespace Ui {

class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  void fullscreen();

  void openSettings();

  void quit();

  void startGame();

  void backToMenu();

  void backToMenuInit();  // go to main menu and initialize a new game

  void menuToStatistics();

  void settingsToMenu();

  void initNewGame();

  void dealNewGame();

  int getVolume();

  void resizeEvent(QResizeEvent *event);

  signals:
  void settingsSignal(Settings gameSettings);

 private:
  Ui::MainWindow *ui;
  QStackedWidget *stackedWidget;  // Main window's pages
  GameView *gameView = nullptr;   // Game view page
  Settings gameSettings;
  void switchToPage(int pageIndex);

 private slots:
  void onGameWon(int points);
};

#endif  // MAINWINDOW_H
