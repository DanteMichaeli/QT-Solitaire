#include "mainwindow.h"

#include "game.hpp"
#include "stats.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  ui->menuGame->menuAction()->setVisible(
      false);  // hide gameview topbar menu option

  resize(1400, 900);  // window size

  stackedWidget = ui->stackedWidget;
  stackedWidget->setCurrentIndex(0);
  // topbar connections
  connect(ui->actionFullscreen, &QAction::triggered, this,
          &MainWindow::fullscreen);
  connect(ui->actionQuit, &QAction::triggered, this, &QApplication::quit);

  connect(ui->actionNew_Game, &QAction::triggered, this,
          &MainWindow::dealNewGame);

  // Main menu button connections

  connect(ui->startGameButton, &QPushButton::clicked, this,
          &MainWindow::startGame);

  connect(ui->settingsButton, &QPushButton::clicked, this,
          &MainWindow::openSettings);

  connect(ui->statsButton, &QPushButton::clicked, this,
          &MainWindow::menuToStatistics);

  connect(ui->quitButton, &QPushButton::clicked, this, &MainWindow::quit);

  // settings menu button connections
  connect(ui->exitSettingsButton, &QPushButton::clicked, this,
          &MainWindow::settingsToMenu);

  connect(ui->winToMenuButton, &QPushButton::clicked, this,
          &MainWindow::backToMenuInit);

  // stat to menu button
  connect(ui->statsToMenuButton, &QPushButton::clicked, this,
          &MainWindow::backToMenu);

  // get initial settings
  gameSettings.isHardModeEnabled = ui->hardModeCheckbox->isChecked();
  gameSettings.isHintsEnabled = ui->hintsCheckbox->isChecked();
  gameSettings.volume = ui->volumeSlider->value();

  initNewGame();
}

void MainWindow::fullscreen()  // Fullscreen button functionality
{
  if (windowState() & Qt::WindowFullScreen) {
    // If the window is already fullscreen, exit fullscreen
    setWindowState(Qt::WindowMaximized);
  } else {
    // Otherwise, enter fullscreen
    setWindowState(Qt::WindowFullScreen);
  }
}

void MainWindow::switchToPage(int pageIndex) {
  stackedWidget->setCurrentIndex(pageIndex);
}

void MainWindow::backToMenu() {
  ui->menuGame->menuAction()->setVisible(false);
  switchToPage(0);
}

void MainWindow::settingsToMenu() {
  gameSettings.isHardModeEnabled = ui->hardModeCheckbox->isChecked();
  gameSettings.isHintsEnabled = ui->hintsCheckbox->isChecked();
  gameSettings.volume = ui->volumeSlider->value();
  emit settingsSignal(gameSettings);
  switchToPage(0);
}

void MainWindow::backToMenuInit() {
  backToMenu();
  initNewGame();
}

void MainWindow::menuToStatistics() {
  GameStats playStats = fromCSV("stats.csv");

  ui->winsLCD->display(playStats.wins);
  ui->lossesLCD->display(playStats.losses);
  ui->winRateLCD->display(playStats.winRate);
  ui->averageTimeLCD->display(playStats.avgTime);
  ui->hintCountLCD->display(playStats.hintCount);
  ui->averageMovesLCD->display(playStats.avgMoves);
  ui->undoCountLCD->display(playStats.undoCount);
  ui->averagePointsLCD->display(playStats.avgPoints);
  ui->bestPointsLCD->display(playStats.bestPoints);
  ui->leaveRateLCD->display(playStats.leaveRate);
  ui->gamesLCD->display(playStats.games);

  switchToPage(4);
}

void MainWindow::startGame() {
  switchToPage(2);
  ui->menuGame->menuAction()->setVisible(true);
  QSizeF size = this->size();
  gameView->updateLayout(size);
}

void MainWindow::initNewGame() {
  // Check if gameView already exists
  if (gameView != nullptr) {
    // Remove gameView from the stacked widget
    int index = stackedWidget->indexOf(gameView);
    if (index != -1) {
      stackedWidget->removeWidget(gameView);
    }
    // Delete the old gameView
    delete gameView;
    gameView = nullptr;
  }

  // Create a new GameView
  gameView = new GameView(this, getVolume());
  connect(gameView, &GameView::gameWon, this, &MainWindow::onGameWon);

  // Insert the new GameView into the stacked widget
  stackedWidget->insertWidget(2, gameView);  // Insert GameView at index 2

  // connect settings to new game
  connect(this, &MainWindow::settingsSignal, gameView,
          &GameView::settingsRelaySlot);
  emit settingsSignal(gameSettings);
}

void MainWindow::dealNewGame() {
  initNewGame();
  switchToPage(2);
}

void MainWindow::openSettings() { switchToPage(1); }

void MainWindow::onGameWon(int points) {
  ui->pointsCounter->display(points);
  switchToPage(3);
}

int MainWindow::getVolume() { return ui->volumeSlider->value(); }

void MainWindow::quit() { this->close(); }

MainWindow::~MainWindow() { delete ui; }

void MainWindow::resizeEvent(QResizeEvent *event) {
  QSizeF newSize = event->size();
  if (gameView != nullptr) {
    gameView->updateLayout(newSize);
  }
  QMainWindow::resizeEvent(event);
}
