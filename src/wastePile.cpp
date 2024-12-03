#include "wastePile.hpp"

#include <QDebug>

#include "deck.hpp"

WastePile::WastePile(QGraphicsItem* parent) : Pile(parent) {}

bool WastePile::IsValid(const Card& card) { return false; }

int WastePile::AddFromDeck(Deck& deck, size_t nofCards) {
  for (size_t i = 0; i < nofCards; i++) {
    auto cardPtr = deck.RemoveCard();
    if (cardPtr == nullptr) {
      return i;
    }
    cardPtr->flipUp();
    this->AddCard(cardPtr);
  }
  return nofCards;
}

void WastePile::undoAddFromDeck(Deck& deck, size_t nofCards) {
  for (size_t i = 0; i < nofCards; i++) {
    auto cardPtr = this->RemoveCard();
    if (cardPtr == nullptr) {
      break;
    }
    cardPtr->flipDown();
    deck.AddCard(cardPtr);
  }
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
