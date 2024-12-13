#include "gameView.hpp"
#define QT_DEBUG

#include <QDebug>
#include <QHBoxLayout>
#include <QMenu>
#include <QVBoxLayout>
#include <QWidgetAction>

#include "klondikeLayout.hpp"
#include "mainwindow.h"

GameView::GameView(Settings &settings, QWidget *parent)
    : QGraphicsView(parent),
      scene_(new QGraphicsScene(this)),
      game_(make_unique<Game>()) {
  initView();
  initButtons();
  initLabels();
  initToolbar();
  changeSettings(settings);
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

  layout_ = make_unique<KlondikeLayout>(scene_, game_.get());
  connect(game_.get(), &Game::gameWon, this, &GameView::handleGameWon);
}

void GameView::initButtons() {
  // Set proper parent-child ownership for buttons
  undoButton_ = new QPushButton("Undo", this);
  connect(undoButton_, &QPushButton::clicked, this,
          [this]() { game_->undo(); });

  hintButton_ = new QPushButton("Hint", this);
  connect(hintButton_, &QPushButton::clicked, this,
          [this]() { game_->hint(); });

  // Create a menu button
  menuButton_ = new QToolButton();
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
  connect(newGameButton, &QPushButton::clicked, this, [this, menu]() {
    menu->close();
    emit dropdownSignal(DD_NEW_GAME);
  });
  connect(mainMenuButton, &QPushButton::clicked, this, [this, menu]() {
    menu->close();
    emit dropdownSignal(DD_MAIN_MENU);
  });
  connect(settingsButton, &QPushButton::clicked, this, [this, menu]() {
    menu->close();
    emit dropdownSignal(DD_SETTINGS);
  });
  connect(quitButton, &QPushButton::clicked, this, [this, menu]() {
    menu->close();
    emit dropdownSignal(DD_QUIT);
  });

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

  connect(game_.get(), &Game::gameStateChange, this,
          &GameView::handleGameStateChange);
  connect(game_.get(), &Game::updateTime, this, &GameView::handleTimeElapsed);
}

void GameView::initToolbar() {
  // Create a custom toolbar widget
  toolbarWidget_ = new QWidget(this);

  QHBoxLayout *toolbarLayout = new QHBoxLayout(toolbarWidget_);
  toolbarLayout->setContentsMargins(10, 10, 50, 10);
  toolbarLayout->setSpacing(20);

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

void GameView::updateLayout(const QSizeF &newSize) {
  layout_->resize(newSize);
  updateToolbarSize(newSize);
}

void GameView::changeSettings(const Settings &gameSettings) {
  game_->changeSettings(gameSettings);
  if (!gameSettings.isHintsEnabled) {
    hintButton_->setEnabled(false);
    hintButton_->setStyleSheet("background-color: lightgray; color: gray;");
  } else {
    hintButton_->setEnabled(true);
    hintButton_->setStyleSheet(
        "background-color: #964B00; border-bottom: 2px solid #222222; color: "
        "white;");
  }
}

void GameView::updateToolbarSize(const QSizeF &size) {
  if (toolbarWidget_) {
    int toolbarWidth = size.width();
    int toolbarHeight = 40;  // Fixed height for the toolbar

    // Set the size of the toolbar
    toolbarWidget_->setFixedSize(toolbarWidth, toolbarHeight);
  }
}

void GameView::handleGameStateChange(const unsigned int points,
                                     const unsigned int moves) {
  pointsLabel_->setText(QString("Points: %1").arg(points));
  moveLabel_->setText(QString("Moves: %1").arg(moves));
}

void GameView::handleGameWon(const unsigned int points) {
  emit gameWon(points);
}

void GameView::handleTimeElapsed(const unsigned int elapsedTime) {
  QString newText = MainWindow::formatTime(elapsedTime);
  timerLabel_->setText(newText);
}
