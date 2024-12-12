#include "mainwindow.h"

#include <QTime>

#include "game.hpp"
#include "stats.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      gameView_(nullptr),
      gameStarted_(false) {
  ui->setupUi(this);

  ui->menuGame->menuAction()->setVisible(
      false);  // hide gameview topbar menu option

  resize(1400, 900);  // window size

  stackedWidget_ = ui->stackedWidget;
  stackedWidget_->setCurrentIndex(MENU);

  // topbar connections
  connect(ui->actionFullscreen, &QAction::triggered, this,
          &MainWindow::fullscreen);
  connect(ui->actionQuit, &QAction::triggered, this, &QApplication::quit);
  connect(ui->actionNew_Game, &QAction::triggered, this,
          &MainWindow::startGame);

  // Main menu button connections

  connect(ui->continueButton, &QPushButton::clicked, this,
          &MainWindow::continueGame);

  connect(ui->startGameButton, &QPushButton::clicked, this,
          &MainWindow::startGame);

  connect(ui->settingsButton, &QPushButton::clicked, this,
          &MainWindow::openSettings);

  connect(ui->statsButton, &QPushButton::clicked, this,
          &MainWindow::toStatistics);

  connect(ui->quitButton, &QPushButton::clicked, this, &MainWindow::quit);

  // settings menu button connections
  connect(ui->exitSettingsButton, &QPushButton::clicked, this,
          &MainWindow::returnToPrev);

  connect(ui->winToMenuButton, &QPushButton::clicked, this,
          &MainWindow::backToMenuInit);

  // stat to menu button
  connect(ui->statsToMenuButton, &QPushButton::clicked, this,
          &MainWindow::toMenu);

  loadSettings();
  initNewGame();
}

void MainWindow::loadSettings() {
  QString filepath("settings.json");

  if (!loadSettingsFromJSON(gameSettings_, filepath)) {
    qDebug() << "Initializing a new settings.JSON file.";
    gameSettings_.volume = 50;
    gameSettings_.isHardModeEnabled = false;
    gameSettings_.isHintsEnabled = true;
    saveSettingsToJSON(gameSettings_, filepath);
  }
  ui->hardModeCheckbox->setChecked(gameSettings_.isHardModeEnabled);
  ui->hintsCheckbox->setChecked(gameSettings_.isHintsEnabled);
  ui->volumeSlider->setValue(gameSettings_.volume);
}

void MainWindow::saveSettings() {
  gameSettings_.isHardModeEnabled = ui->hardModeCheckbox->isChecked();
  gameSettings_.isHintsEnabled = ui->hintsCheckbox->isChecked();
  gameSettings_.volume = ui->volumeSlider->value();

  QString filepath("settings.json");
  saveSettingsToJSON(gameSettings_, filepath);

  if (gameView_) gameView_->changeSettings(gameSettings_);
}

void MainWindow::fullscreen() {
  if (windowState() & Qt::WindowFullScreen) {
    setWindowState(Qt::WindowMaximized);
  } else {
    setWindowState(Qt::WindowFullScreen);
  }
}

void MainWindow::openSettings() { switchToPage(SETTINGS); }

void MainWindow::toMenu() {
  ui->menuGame->menuAction()->setVisible(false);
  if (gameStarted_) {
    ui->continueButton->setEnabled(true);
  } else {
    ui->continueButton->setEnabled(false);
  }
  switchToPage(MENU);
}

void MainWindow::switchToPage(Window window) {
  previousWindow_ = static_cast<Window>(stackedWidget_->currentIndex());
  if (previousWindow_ == SETTINGS) saveSettings();
  stackedWidget_->setCurrentIndex(window);
}

void MainWindow::returnToPrev() { switchToPage(previousWindow_); }

void MainWindow::backToMenuInit() {
  toMenu();
  initNewGame();
  ui->continueButton->setEnabled(false);
}

void MainWindow::toStatistics() {
  GameStats playStats = fromCSV("stats.csv");

  ui->gamesLCD->display(QString::number(playStats.games));
  ui->winsLCD->display(QString::number(playStats.wins));
  ui->lossesLCD->display(QString::number(playStats.losses));
  ui->winRateLCD->display(QString::number(playStats.winRate, 'f', 2));

  QString tot = MainWindow::formatTime(playStats.totalTime);
  QString best = MainWindow::formatTime(playStats.bestTime);
  QString avg = MainWindow::formatTime(playStats.avgTime);

  ui->totalTimeLCD->display(tot);
  ui->bestTimeLCD->display(best);
  ui->averageTimeLCD->display(avg);

  ui->totalMovesLCD->display(QString::number(playStats.totalMoves));
  ui->bestMovesLCD->display(QString::number(playStats.bestMoves));
  ui->averageMovesLCD->display(QString::number(playStats.avgMoves, 'f', 2));

  ui->hintCountLCD->display(QString::number(playStats.hintCount));
  ui->undoCountLCD->display(QString::number(playStats.undoCount));

  ui->totalPointsLCD->display(QString::number(playStats.totalPoints));
  ui->bestPointsLCD->display(QString::number(playStats.bestPoints));
  ui->averagePointsLCD->display(QString::number(playStats.avgPoints, 'f', 2));

  switchToPage(STATS);
}

void MainWindow::startGame() {
  if (gameStarted() || !gameInitialized()) initNewGame();

  QSizeF size = this->size();
  gameView_->updateLayout(size);
  gameView_->startGame();
  setGameStarted(true);

  ui->menuGame->menuAction()->setVisible(true);
  switchToPage(GAME);
}

void MainWindow::continueGame() {
  if (gameInitialized()) {
    ui->menuGame->menuAction()->setVisible(true);
    switchToPage(GAME);
  }
}

void MainWindow::deleteGame() {
  if (gameInitialized()) {
    stackedWidget_->removeWidget(gameView_);
    gameView_->deleteLater();
    gameView_ = nullptr;
    setGameStarted(false);
  }
}

void MainWindow::initNewGame() {
  deleteGame();

  gameView_ = new GameView(gameSettings_, this);
  connect(gameView_, &GameView::gameWon, this, &MainWindow::onGameWon);
  connect(gameView_, &GameView::dropdownSignal, this,
          &MainWindow::fromDropdownSlot);

  // Insert the new GameView into the stacked widget
  stackedWidget_->insertWidget(GAME, gameView_);
}

void MainWindow::onGameWon(int points) {
  ui->pointsCounter->display(points);
  switchToPage(WIN_SCREEN);
}

void MainWindow::quit() { this->close(); }

MainWindow::~MainWindow() { delete ui; }

void MainWindow::fromDropdownSlot(DropDownOption option) {
  switch (option) {
    case DD_NEW_GAME:
      startGame();
      break;
    case DD_MAIN_MENU:
      toMenu();
      break;
    case DD_SETTINGS:
      openSettings();
      break;
    case DD_QUIT:
      quit();
      break;
    default:
      return;
  }
}

bool MainWindow::gameInitialized() { return gameView_ != nullptr; }

bool MainWindow::gameStarted() { return gameStarted_; }

void MainWindow::setGameStarted(bool value) { gameStarted_ = value; }

QString MainWindow::formatTime(size_t seconds) {
  size_t h = seconds / 3600;
  size_t min = (seconds % 3600) / 60;
  size_t sec = seconds % 60;

  return QString("%1:%2:%3")
      .arg(h)
      .arg(min, 2, 10, QChar('0'))
      .arg(sec, 2, 10, QChar('0'));
}

void MainWindow::resizeEvent(QResizeEvent *event) {
  QSizeF newSize = event->size();
  if (gameView_ != nullptr) {
    gameView_->updateLayout(newSize);
  }
  QMainWindow::resizeEvent(event);
}
