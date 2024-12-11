#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QResizeEvent>
#include <QStackedWidget>

#include "gameView.hpp"
#include "settings.hpp"

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

 private:
  Ui::MainWindow *ui;
  QStackedWidget *stackedWidget;  // Main window's pages
  GameView *gameView;             // Game view page
  Settings gameSettings;
  bool gameStarted_;
  void switchToPage(int pageIndex);

 private slots:
  void onGameWon(int points);
  void fromDropdownSlot(DropDownOption option);
};

#endif  // MAINWINDOW_H
