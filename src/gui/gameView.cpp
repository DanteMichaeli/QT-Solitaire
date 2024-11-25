#include "gameView.hpp"

#include <QDebug>

#include "klondikePile.hpp"
#include "targetPile.hpp"
#include "wastePile.hpp"

GameView::GameView(QWidget* parent)
    : QGraphicsView(parent),
      scene(new QGraphicsScene(this)),
      klondikePiles(7) {  // Predefine vector size
  setScene(scene);
  setAlignment(Qt::AlignLeft | Qt::AlignTop);
  setBackgroundBrush(QBrush(Qt::green));

  initializeGame();
}

void GameView::initializeGame() {
  scene->clear();

  // Initialize the deck
  deck_ = std::make_unique<Deck>();
  scene->addItem(deck_.get());
  deck_->setScale(0.2);
  deck_->setPos(70, 70);

  qDebug() << "Deck initialized with 52 cards.";

  // Create and add all seven Klondike piles to the tableau
  for (int i = 0; i < 7; i++) {
    qDebug() << "Deck size before creating pile" << i + 1 << ":" << 52 - i;

    qDebug() << "Creating pile" << i + 1;

    if (deck_->Empty()) {
      qDebug() << "Error: Deck is empty while creating pile" << i + 1;
      return;
    }

    // Init KlondikePile
    klondikePiles[i] = (std::make_unique<KlondikePile>(*deck_, i + 1, 1));

    qDebug() << "Pile" << i + 1 << "created with" << i + 1 << "cards.";

    // Create the graphical item for the pile and add it to the scene
    scene->addItem(klondikePiles[i].get());
    klondikePiles[i].get()->setPos(200 + i * 150, 150);

    qDebug() << "Pile" << i + 1 << "added to the scene.";
  }
}
