#include "wastePile.hpp"

#include <QDebug>

#include "deck.hpp"

WastePile::WastePile(QGraphicsItem* parent) : Pile(parent) {}

bool WastePile::IsValid(const Card& card) { return false; }

int WastePile::AddFromDeck(Deck& deck, size_t nofCards) {
  size_t i = 0;
  while (i < nofCards && !deck.Empty()) {
    deck.getTopCard()->flipUp();
    deck.TransferCard(*this);
    i++;
  }
  return i;
}

void WastePile::undoAddFromDeck(Deck& deck, size_t nofCards) {
  size_t i = 0;
  while (i < nofCards && !Empty()) {
    getTopCard()->flipDown();
    TransferCard(deck);
    i++;
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
    cards_[i]->setPos(j * PILE_YOFFSET, 0);
    cards_[i]->show();
    i++;
    j++;
  }
}
