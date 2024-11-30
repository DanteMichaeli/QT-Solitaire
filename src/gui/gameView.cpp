#include "gameView.hpp"

#include <QDebug>

#include "klondikePile.hpp"
#include "targetPile.hpp"
#include "wastePile.hpp"

GameView::GameView(QWidget* parent)
    : QGraphicsView(parent),
      scene(new QGraphicsScene(this)),
      game_(std::make_unique<Game>()) {
  setScene(scene);
  setAlignment(Qt::AlignLeft | Qt::AlignTop);
  setBackgroundBrush(QBrush(Qt::green));
  scene->setSceneRect(scene->itemsBoundingRect());
  initializeGame();
  viewport()->update();
}

void GameView::initializeGame() {
  scene->clear();

  // Initialize the deck
  Deck* deck = game_->getDeck();
  scene->addItem(deck);
  deck->setPos(70, 70);
  qDebug() << "Deck initialized with 52 cards.";

  WastePile* wastePile = game_->getWPile();
  scene->addItem(wastePile);
  wastePile->setPos(70, 250);

  // Create and add all seven Klondike piles to the tableau

  auto& kPiles = game_->getKPiles();
  size_t i = 0;
  for (auto& ptr : kPiles) {
    scene->addItem(ptr.get());
    ptr.get()->setPos(200 + i * 150, 150);
    i++;
    qDebug() << "KlondikePile" << i << "added to the scene.";
  }

  auto& tPiles = game_->getTPiles();
  qDebug() << tPiles.size();
  i = 0;
  for (auto& ptr : tPiles) {
    scene->addItem(ptr.get());
    ptr.get()->setPos(1250, 150 + 160 * i);
    i++;
    qDebug() << "KlondikePile" << i << "added to the scene.";
  }
}
