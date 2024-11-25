#include "wastePile.hpp"

WastePile::WastePile(QGraphicsItem* parent) : Pile(parent) {}

void WastePile::AddFromDeck(Deck& deck, size_t nofCards) {
  for (size_t i = 0; i < nofCards; i++) {
    auto cardPtr = deck.dealCard();
    if (cardPtr == nullptr) {
      break;
    }
    cardPtr->flipUp();
    cards_.push_back(std::move(cardPtr));
  }
}

bool WastePile::AddToDeck(Deck& deck, bool shuffle) {
  if (deck.Empty()) {
    while (!cards_.empty()) {
      deck.AddCard(std::move(cards_.back()));
      cards_.pop_back();
    }
    if (shuffle) {
      deck.Shuffle();
    }
    return true;
  }
  return false;
}

// TODO:
void WastePile::mousePressEvent(QGraphicsSceneMouseEvent* event) {}