#include "gameView.hpp"
#define QT_DEBUG

#include <QDebug>
#include <QGraphicsProxyWidget>
#include <QHBoxLayout>
#include <QMenu>
#include <QPushButton>
#include <QTime>
#include <QVBoxLayout>
#include <QWidgetAction>

#include "klondikeLayout.hpp"
#include "klondikePile.hpp"
#include "mainwindow.h"
#include "targetPile.hpp"
#include "wastePile.hpp"

GameView::GameView(QWidget *parent, int volume)
    : QGraphicsView(parent),
      scene_(new QGraphicsScene(this)),
      game_(new Game(this)) {
  initView();
  initButtons();
  initLabels();
  initToolbar();
}

void GameView::initView() {
  setScene(scene_);
  setSceneRect(0, 0, width(), height());
  setAlignment(Qt::AlignLeft | Qt::AlignTop);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setBackgroundBrush(QBrush(QColor(0x3C9B3F)));
  this->setRenderHint(QPainter::Antialiasing, true);
  scene_->clear();

  layout_ = make_unique<KlondikeLayout>(scene_, game_);
  connect(game_, &Game::gameWon, this, &GameView::handleGameWon);
}

void GameView::initButtons() {
  // Set proper parent-child ownership for buttons
  undoButton_ = new QPushButton("Undo", this);  // Pass 'this' as parent
  connect(undoButton_, &QPushButton::clicked, this,
          [this]() { game_->undo(); });

  hintButton_ = new QPushButton("Hint", this);  // Pass 'this' as parent
  connect(hintButton_, &QPushButton::clicked, this,
          [this]() { game_->hint(); });

  // Create a menu button and set parent to 'this'
  menuButton_ = new QToolButton();  // Pass 'this' as parent
  menuButton_->setText("Options");
  menuButton_->setIcon(QIcon::fromTheme("go-home"));
  menuButton_->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

  // Create a dropdown menu
  QMenu *menu = new QMenu(menuButton_);
  QWidget *widgetMenu = new QWidget(menu);
  QVBoxLayout *menuLayout = new QVBoxLayout(widgetMenu);
  menuLayout->setContentsMargins(5, 10, 5, 10);

  QPushButton *newGameButton = new QPushButton("New game");
  QPushButton *mainMenuButton = new QPushButton("Main menu");
  QPushButton *settingsButton = new QPushButton("Settings");
  QPushButton *quitButton = new QPushButton("Quit");

  // Connect actions with the same signal but different values
  connect(newGameButton, &QPushButton::clicked, this,
          [this]() { emit dropdownSignal(NEW_GAME); });
  connect(mainMenuButton, &QPushButton::clicked, this,
          [this]() { emit dropdownSignal(MAIN_MENU); });
  connect(settingsButton, &QPushButton::clicked, this,
          [this]() { emit dropdownSignal(SETTINGS); });
  connect(quitButton, &QPushButton::clicked, this,
          [this]() { emit dropdownSignal(QUIT); });

  // Add buttons to the layout
  menuLayout->addWidget(newGameButton);
  menuLayout->addWidget(mainMenuButton);
  menuLayout->addWidget(settingsButton);
  menuLayout->addWidget(quitButton);

  // Add the widget to the menu as a QWidgetAction
  QWidgetAction *widgetAction = new QWidgetAction(menu);
  widgetAction->setDefaultWidget(widgetMenu);
  menu->addAction(widgetAction);

  // Link the menu with the button
  menuButton_->setMenu(menu);
  menuButton_->setPopupMode(QToolButton::InstantPopup);

  // Remove the menu indicator
  menuButton_->setStyleSheet(
      "QToolButton::menu-indicator {width: 0px; height: 0px;}");

  widgetMenu->setStyleSheet(
      "background-color: #964B00; "
      "border-bottom: 0px solid #000000; "
      "color: white;");
}

void GameView::initLabels() {
  pointsLabel_ = new QLabel("Points: 0");
  moveLabel_ = new QLabel("Moves: 0");
  timerLabel_ = new QLabel("0:00:00");
  pointsLabel_->setStyleSheet("color: white;");
  moveLabel_->setStyleSheet("color: white;");
  timerLabel_->setStyleSheet("color: white;");

  connect(game_, &Game::gameStateChange, this,
          &GameView::handleGameStateChange);
  connect(game_, &Game::updateTime, this, &GameView::handleTimeElapsed);
}

void GameView::initToolbar() {
  // Create a custom toolbar widget
  toolbarWidget_ = new QWidget(this);

  QHBoxLayout *toolbarLayout = new QHBoxLayout(toolbarWidget_);
  toolbarLayout->setContentsMargins(10, 10, 50, 10);
  toolbarLayout->setSpacing(20);
  QSpacerItem *spacer =
      new QSpacerItem(30, 0, QSizePolicy::Fixed, QSizePolicy::Minimum);

  // Add buttons and labels to the custom toolbar
  toolbarLayout->addWidget(undoButton_);
  toolbarLayout->addWidget(hintButton_);
  toolbarLayout->addStretch();
  toolbarLayout->addWidget(timerLabel_);
  toolbarLayout->addWidget(pointsLabel_);
  toolbarLayout->addWidget(moveLabel_);
  toolbarLayout->addWidget(menuButton_);

  // Set the style of the toolbar widget
  toolbarWidget_->setStyleSheet(
      "background-color: #964B00; "
      "border-bottom: 2px solid #222222; "
      "color: white;");

  toolbarWidget_->setLayout(toolbarLayout);
}

void GameView::updateLayout(QSizeF &newSize) {
  layout_->resize(newSize);
  updateToolbarSize(newSize);
}

void GameView::settingsRelaySlot(Settings gameSettings) {
  game_->updateSettingsSlot(gameSettings);
  if (!gameSettings.isHintsEnabled) {
    hintButton_->setEnabled(false);
    hintButton_->setStyleSheet("background-color: lightgray; color: gray;");
  } else {
    hintButton_->setEnabled(true);
    hintButton_->setStyleSheet(
        "background-color: #964B00; border-bottom: 2px solid #222222; color: "
        "white;");
  }

  qDebug() << "relaying settings";
}

void GameView::updateToolbarSize(QSizeF &size) {
  if (toolbarWidget_) {
    // Get the current scene size or view size
    int toolbarWidth = size.width();
    int toolbarHeight = 40;  // Fixed height for the toolbar

    // Set the size of the toolbar
    toolbarWidget_->setFixedSize(toolbarWidth, toolbarHeight);
  }
}

void GameView::handleGameStateChange(int points, int moves) {
  pointsLabel_->setText(QString("Points: %1").arg(points));
  moveLabel_->setText(QString("Moves: %1").arg(moves));
}

void GameView::handleGameWon(int points) { emit gameWon(points); }

void GameView::handleTimeElapsed(unsigned long elapsedTime) {
  QString newText = MainWindow::formatTime(elapsedTime);
  timerLabel_->setText(newText);
}
