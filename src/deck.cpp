#include "deck.hpp"

#include <QDebug>

#include "wastePile.hpp"

Deck::Deck(QGraphicsItem* parent) : Pile(parent) {
  // Add cards to deck
  for (Suit suit : allSuits) {
    for (Rank rank : allRanks) {
      auto card = std::make_unique<Card>(suit, rank);
      // card->hide();
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
}

// TODO:
bool Deck::IsValid(const Card& card) {
  if (static_cast<WastePile*>(card.parentItem()) != nullptr) {
    return true;
  }
  return false;
}

bool Deck::recycle(WastePile& pile) {
  if (this->Empty() && !pile.Empty()) {
    while (!pile.Empty()) {
      auto cardPtr = pile.RemoveCard();
      cardPtr->flipDown();
      this->AddCard(cardPtr);
    }
    return true;
  }
  return false;
}

void Deck::undoRecycle(WastePile& pile) {
  while (!this->Empty()) {
    auto cardPtr = RemoveCard();
    cardPtr->flipUp();
    pile.AddCard(cardPtr);
  }
}

void Deck::updateVisuals() {}

void Deck::mousePressEvent(QGraphicsSceneMouseEvent* event) {
  emit cardClickMove(nullptr, this);
}
