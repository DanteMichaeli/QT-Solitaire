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
          &MainWindow::startGame);

  // Main menu button connections

  connect(ui->startGameButton, &QPushButton::clicked, this,
          &MainWindow::startGame);

  connect(ui->settingsButton, &QPushButton::clicked, this,
          &MainWindow::openSettings);
  connect(ui->quitButton, &QPushButton::clicked, this, &MainWindow::quit);

  // settings menu button connections
  connect(ui->exitSettingsButton, &QPushButton::clicked, this,
          &MainWindow::backToMenu);

  connect(ui->winToMenuButton, &QPushButton::clicked, this,
          &MainWindow::backToMenu);
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

void MainWindow::startGame() {
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
  gameView = new GameView(this);
  connect(gameView, &GameView::gameWon, this, &MainWindow::onGameWon);

  // Insert the new GameView into the stacked widget
  stackedWidget->insertWidget(2, gameView);  // Insert GameView at index 2

  // Switch to the game page
  switchToPage(2);
  QSizeF size = this->size();
  gameView->updateLayout(size);
}

void MainWindow::openSettings() { switchToPage(1); }

void MainWindow::onGameWon(int points) {
  ui->pointsCounter->display(points);
  switchToPage(3);
}

void MainWindow::quit() { this->close(); }

MainWindow::~MainWindow() { delete ui; }

void MainWindow::resizeEvent(QResizeEvent *event) {
  QSizeF newSize = event->size();
  if (gameView != nullptr) {
    gameView->updateLayout(newSize);
  }
  QMainWindow::resizeEvent(event);
}