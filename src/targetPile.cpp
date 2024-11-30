#include "targetPile.hpp"

#include <QDebug>

TargetPile::TargetPile(Suit suit, QGraphicsItem* parent)
    : Pile(parent), suit_(suit) {
  qDebug() << "Created TargetPile";
}

bool TargetPile::IsValid(const Card& card) {
  if (!card.isFaceUp()) {
    return false;
  }
  if (card.GetSuit() != suit_) {
    return false;
  }
  if (Empty()) {
    return card.GetRank() == Rank::ACE;
  } else {
    return card.GetRank() == cards_.back()->GetRank() + 1;
  }
}

void TargetPile::updateVisuals() {
  // Render only top 2 cards (potentially for smooth animation)
  int size = Size();
  if (size == 0) {
    return;
  } else if (size <= 2) {
    cards_[size - 1]->setPos(0, 0);
  } else {
    cards_[size - 1]->setPos(0, 0);
    cards_[size - 3]->hide();
  }
}