#include "wastePile.hpp"

WastePile::WastePile(QGraphicsItem* parent) : Pile(parent) {}

bool WastePile::IsValid(const Card& card) { return false; }

void WastePile::AddFromDeck(Deck& deck, size_t nofCards) {
  for (size_t i = 0; i < nofCards; i++) {
    auto cardPtr = deck.RemoveCard();
    if (cardPtr == nullptr) {
      break;
    }
    cardPtr->flipUp();
    this->AddCard(cardPtr);
  }
}

bool WastePile::AddToDeck(Deck& deck, bool shuffle) {
  if (deck.Empty()) {
    while (!cards_.empty()) {
      auto cardPtr = this->RemoveCard();
      cardPtr->flipDown();
      deck.AddCard(cardPtr);
    }
    if (shuffle) {
      deck.Shuffle();
    }
    return true;
  }
  return false;
}

void WastePile::updateVisuals() {
  int size = cards_.size();
  int i = 0;
  while (i < size - 3) {
    cards_[i]->hide();
    i++;
  }
  int j = 0;
  while (i < size) {
    cards_[i]->setPos(0, j * PILE_YOFFSET);
    cards_[i]->show();
    i++;
    j++;
  }
}
