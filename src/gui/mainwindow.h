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

enum Window { MENU, SETTINGS, GAME, WIN_SCREEN, STATS };

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = nullptr);

  ~MainWindow();

  void fullscreen();

  void openSettings();

  void quit();

  void toMenu();

  void returnToPrev();

  void backToMenuInit();

  void toStatistics();

  void startGame();

  void continueGame();

  void deleteGame();

  void initNewGame();

  bool gameInitialized();

  bool gameStarted();

  void setGameStarted(bool value);

  static QString formatTime(size_t seconds);

 private:
  Ui::MainWindow *ui;
  QStackedWidget *stackedWidget_;  // Main window's pages
  GameView *gameView_;             // Game view page
  Settings gameSettings_;
  Window previousWindow_;

  bool gameStarted_;

  void switchToPage(Window window);

  void loadSettings();

  void saveSettings();

  void resizeEvent(QResizeEvent *event) override;

 private slots:
  void onGameWon(int points);
  void fromDropdownSlot(DropDownOption option);
};

#endif  // MAINWINDOW_H
