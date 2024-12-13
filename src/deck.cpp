#include "deck.hpp"

#include <QDebug>
#include <algorithm>
#include <chrono>
#include <random>
#include <vector>

#include "wastePile.hpp"

Deck::Deck(QGraphicsItem* parent) : Pile(parent) {
  // Add cards to deck
  std::vector<Card*> aux;
  for (Suit suit : allSuits) {
    for (Rank rank : allRanks) {
      aux.push_back(new Card(suit, rank));
    }
  }

  Deck::shuffle<Card*>(aux);
  for (auto& card : aux) this->addCard(card);
}

// LOGIC RELATED FUNCTIONS

template <typename T>
void Deck::shuffle(std::vector<T>& arr, unsigned long seed) {
  if (seed == 0) {
    // Default seed is based on system time.
    seed = chrono::high_resolution_clock::now().time_since_epoch().count();
  }
  mt19937 generator(seed);
  std::shuffle(arr.begin(), arr.end(), generator);
}

bool Deck::isValid(const Card& card) { return false; }

bool Deck::recycle(WastePile& pile) {
  if (this->isEmpty() && !pile.isEmpty()) {
    while (!pile.isEmpty()) {
      Card* topCard = pile.getTopCard();
      if (topCard->isFaceUp()) {
        topCard->flip();
      }
      pile.transferCards(*this);
    }
    return true;
  }
  return false;
}

void Deck::undoRecycle(WastePile& pile) {
  while (!this->isEmpty()) {
    Card* topCard = this->getTopCard();
    if (!topCard->isFaceUp()) {
      topCard->flip();
    }
    transferCards(pile);
  }
}

// GUI RELATED FUNCTIONS

void Deck::setCardsPrevScenePos() {
  QPointF scenePos = mapToScene(this->getOffset());
  for (auto& card : cards_) {
    card->setPrevScenePos(scenePos);
  }
}

void Deck::updateVisuals() {
  int i = this->getSize();
  QPointF endPos(0, 0);
  while (i > 0) {
    i--;
    // Get the card's previous position
    Card* card = cards_[i];
    QPointF prevPos = card->getPrevScenePos();
    QPointF startPos = this->mapFromScene(prevPos);

    if (startPos != endPos) {
      // Bring card up, parnet z level ultimately determines rendering order
      this->setZValue(3);
      card->animateMove(startPos, endPos);
    } else {
      break;
    }
  }
}

QPointF Deck::getOffset() const { return QPoint(0, 0); }

// When deck is empty, this event can occur, triggers recycle
void Deck::mousePressEvent(QGraphicsSceneMouseEvent* event) {
  emit cardClickMove(nullptr, this);
}
