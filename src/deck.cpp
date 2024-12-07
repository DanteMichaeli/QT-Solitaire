#include "deck.hpp"

#include <QDebug>

#include "wastePile.hpp"

Deck::Deck(QGraphicsItem* parent) : Pile(parent) {
  // Add cards to deck
  for (Suit suit : allSuits) {
    for (Rank rank : allRanks) {
      auto card = std::make_unique<Card>(suit, rank);
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
    cards_[i]->setZValue(i);
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

void Deck::updateVisuals() {
  if (!Empty()) {
    getTopCard()->show();
  }
}

void Deck::mousePressEvent(QGraphicsSceneMouseEvent* event) {
  emit cardClickMove(nullptr, this);
}
