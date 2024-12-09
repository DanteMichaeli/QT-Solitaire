#include "gameView.hpp"

#include <QDebug>
#include <QGraphicsProxyWidget>
#include <QHBoxLayout>
#include <QPushButton>

#include "klondikeLayout.hpp"
#include "klondikePile.hpp"
#include "targetPile.hpp"
#include "wastePile.hpp"

GameView::GameView(QWidget *parent, int volume)
    : QGraphicsView(parent),
      scene(new QGraphicsScene(this)),
      game_(std::make_unique<Game>()) {
  initializeGame();
  viewport()->update();
}

void GameView::initializeGame() {
  setScene(scene);
  setSceneRect(0, 0, width(), height());
  setAlignment(Qt::AlignLeft | Qt::AlignTop);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setBackgroundBrush(QBrush(QColor(0x3C9B3F)));
  this->setRenderHint(QPainter::Antialiasing, true);
  scene->clear();
  layout_ = make_unique<KlondikeLayout>(scene, game_.get());
  connect(game_.get(), &Game::gameWon, this, &GameView::handleGameWon);

  connect(this, &GameView::settingsRelaySignal, game_.get(), &Game::updateSettingsSlot);

  QPushButton *undoButton = new QPushButton("Undo");
  connect(undoButton, &QPushButton::clicked, this, [&]() { game_->undo(); });

  hintButton = new QPushButton("Hint");
  connect(hintButton, &QPushButton::clicked, this, [&]() { game_->hint(); });

  pointsLabel_ = new QLabel("Points: 0");
  moveLabel_ = new QLabel("Moves: 0");
  connect(game_.get(), &Game::gameStateChange, this,
          &GameView::handleGameStateChange);
  pointsLabel_->setStyleSheet("color: white;");
  moveLabel_->setStyleSheet("color: white;");

  QWidget *toolbarWidget = new QWidget();
  QHBoxLayout *toolbarLayout = new QHBoxLayout(toolbarWidget);
  toolbarLayout->setContentsMargins(5, 5, 5, 5);
  toolbarLayout->setSpacing(10);

  toolbarLayout->addWidget(undoButton);
  toolbarLayout->addWidget(hintButton);
  toolbarLayout->addStretch();
  toolbarLayout->addWidget(moveLabel_);
  toolbarLayout->addWidget(pointsLabel_);

  toolbarProxy_ = scene->addWidget(toolbarWidget);
  toolbarProxy_->setPos(0, 0);
  toolbarWidget->setStyleSheet(
      "background-color: #964B00; "
      "border-bottom: 2px solid #222222; "
      "color: white;");
}

void GameView::updateLayout(QSizeF &newSize) {
  layout_->resize(newSize);
  updateToolbarSize();
}

void GameView::settingsRelaySlot(Settings gameSettings)
{
    emit settingsRelaySignal(gameSettings);

    if(!gameSettings.isHintsEnabled)
    {
        hintButton->setEnabled(false);
        hintButton->setStyleSheet("background-color: lightgray; color: gray;"); //make disabled gray out
    }
    else
    {
        hintButton->setEnabled(true);
        hintButton->setStyleSheet("background-color: #964B00; border-bottom: 2px solid #222222; color: white;"); //if not disable make normal
    }

    qDebug() << "relaying settings";
}

void GameView::updateToolbarSize() {
  if (toolbarProxy_) {
    // Get the current scene size or view size
    int toolbarWidth = this->viewport()->width();
    int toolbarHeight = 40;  // Fixed height for the toolbar

    // Set the size of the toolbar
    toolbarProxy_->widget()->setFixedSize(toolbarWidth, toolbarHeight);
  }
}

void GameView::handleGameStateChange(int points, int moves) {
  pointsLabel_->setText(QString("Points: %1").arg(points));
  moveLabel_->setText(QString("Moves: %1").arg(moves));
}
void GameView::handleGameWon(int points) { emit gameWon(points); }
