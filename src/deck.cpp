#include "deck.hpp"

#include <QDebug>

#include "wastePile.hpp"

Deck::Deck(QGraphicsItem* parent) : Pile(parent) {
  // Add cards to deck
  std::vector<Card*> aux;
  for (Suit suit : allSuits) {
    for (Rank rank : allRanks) {
      aux.push_back(new Card(suit, rank));
    }
  }

  Deck::Shuffle<Card*>(aux);
  for (auto& card : aux) this->AddCard(card);
}

template <typename T>
void Deck::Shuffle(std::vector<T>& arr, unsigned long seed) {
  if (seed == 0) {
    // Default seed is based on system time.
    seed = chrono::high_resolution_clock::now().time_since_epoch().count();
  }
  mt19937 generator(seed);
  shuffle(arr.begin(), arr.end(), generator);
}

bool Deck::IsValid(const Card& card) { return false; }

bool Deck::recycle(WastePile& pile) {
  if (this->Empty() && !pile.Empty()) {
    while (!pile.Empty()) {
      Card* topCard = pile.getTopCard();
      if (topCard->isFaceUp()) {
        topCard->flip();
      }
      pile.TransferCard(*this);
    }
    return true;
  }
  return false;
}

void Deck::undoRecycle(WastePile& pile) {
  while (!this->Empty()) {
    Card* topCard = this->getTopCard();
    if (!topCard->isFaceUp()) {
      topCard->flip();
    }
    TransferCard(pile);
  }
}

void Deck::setCardPrevScenePos() {
  QPointF scenePos = mapToScene(this->getOffset());
  for (auto& card : cards_) {
    card->setPrevScenePos(scenePos);
  }
}

void Deck::updateVisuals() {
  int i = Size();
  QPointF endPos(0, 0);
  QPointF endScenePos = mapToScene(endPos);
  while (i > 0) {
    i--;
    // Get the card's previous position
    Card* card = cards_[i];
    QPointF prevPos = card->getPrevScenePos();
    QPointF startPos = this->mapFromScene(prevPos);

    if (startPos != endPos) {
      this->setZValue(3);
      card->animateMove(startPos, endPos);
    } else {
      break;
    }
  }
}

QPointF Deck::getOffset() const { return QPoint(0, 0); }

void Deck::mousePressEvent(QGraphicsSceneMouseEvent* event) {
  emit cardClickMove(nullptr, this);
}
