#include "gameView.hpp"

#include <QDebug>
#include <QGraphicsProxyWidget>
#include <QPushButton>

#include "klondikePile.hpp"
#include "targetPile.hpp"
#include "wastePile.hpp"

GameView::GameView(QWidget *parent)
    : QGraphicsView(parent),
      scene(new QGraphicsScene(this)),
      game_(std::make_unique<Game>()) {
  setScene(scene);
  setAlignment(Qt::AlignLeft | Qt::AlignTop);
  setBackgroundBrush(QBrush(QColor(0x3C9B3F)));
  scene->setSceneRect(scene->itemsBoundingRect());
  initializeGame();
  viewport()->update();
}

void GameView::initializeGame() {
  scene->clear();

  // Initialize the deck
  Deck *deck = game_->getDeck();
  scene->addItem(deck);
  deck->setPos(70, 70);
  qDebug() << "Deck initialized with 52 cards.";

  WastePile *wastePile = game_->getWPile();
  scene->addItem(wastePile);
  wastePile->setPos(70, 250);

  // Create and add all seven Klondike piles to the tableau

  auto &kPiles = game_->getKPiles();
  size_t i = 0;
  for (auto &ptr : kPiles) {
    scene->addItem(ptr.get());
    ptr.get()->setPos(200 + i * 150, 150);
    i++;
  }

  auto &tPiles = game_->getTPiles();
  i = 0;
  for (auto &ptr : tPiles) {
    scene->addItem(ptr.get());
    ptr.get()->setPos(1250, 150 + 160 * i);
    i++;
  }

  // Create the button with text "Undo"
  QPushButton *undoButton = new QPushButton("Undo");

  // Connect the button's clicked signal to a lambda or slot
  connect(undoButton, &QPushButton::clicked, this, [&]() { game_->undo(); });

  // win from game
  connect(game_.get(), &Game::gameWon, this, &GameView::handleGameWon);

  // Add the button to the scene using QGraphicsProxyWidget
  QGraphicsProxyWidget *undoProxyWidget = scene->addWidget(undoButton);
  undoProxyWidget->setPos(70,
                          500);  // Set the position of the button in the scene

  // Create the button with text "Hndo"
  QPushButton *hintButton = new QPushButton("Hint");

  connect(hintButton, &QPushButton::clicked, this, [&]() { game_->hint(); });

  // Add the button to the scene using QGraphicsProxyWidget
  QGraphicsProxyWidget *hintProxyWidget = scene->addWidget(hintButton);
  hintProxyWidget->setPos(70,
                          535);  // Set the position of the button in the scene
}

void GameView::handleGameWon(int points) {
  emit gameWon(points);  // Relay signal to MainWindow.
}
