#include "deck.hpp"

#include <QDebug>

#include "wastePile.hpp"

Deck::Deck(QGraphicsItem* parent) : Pile(parent) {
  // Add cards to deck
  for (Suit suit : allSuits) {
    for (Rank rank : allRanks) {
      Card* card = new Card(suit, rank, this);
      AddCard(card);
    }
  }
}

void Deck::Shuffle(unsigned long seed) {
  if (seed == 0) {
    // Default seed is based on system time.
    seed = chrono::high_resolution_clock::now().time_since_epoch().count();
  }
  mt19937 generator(seed);
  shuffle(cards_.begin(), cards_.end(), generator);

  for (int i = 0; i < Size(); i++) {
    cards_[i]->setZValue(i + 1);
  }
}

bool Deck::IsValid(const Card& card) { return false; }

bool Deck::recycle(WastePile& pile) {
  if (this->Empty() && !pile.Empty()) {
    while (!pile.Empty()) {
      pile.getTopCard()->flipDown();
      pile.TransferCard(*this);
    }
    return true;
  }
  return false;
}

void Deck::undoRecycle(WastePile& pile) {
  while (!this->Empty()) {
    getTopCard()->flipUp();
    TransferCard(pile);
  }
}

void Deck::setCardPrevScenePos() {
  QPointF scenePos = mapToScene(QPointF(0, 1));
  for (auto& card : cards_) {
    card->setPrevScenePos(scenePos);
  }
}

void Deck::updateVisuals() {
  int i = Size();
  QPointF endPos(0, 1);
  QPointF endScenePos = mapToScene(endPos);
  while (i > 0) {
    i--;
    // Get the card's previous position
    Card* card = cards_[i];
    QPointF prevPos = card->getPrevScenePos();
    QPointF startPos = this->mapFromScene(prevPos);

    // Set the start and end positions for the animation
    card->setPrevScenePos(endScenePos);

    // Start the animation
    if (card->pos() != endPos) {
      this->setZValue(6);
      card->animateMove(startPos, endPos);
    } else {
      break;
    }
  }
}

void Deck::mousePressEvent(QGraphicsSceneMouseEvent* event) {
  emit cardClickMove(nullptr, this);
}
