#include "targetPile.hpp"

TargetPile::TargetPile(Suit suit, QGraphicsItem* parent)
    : Pile(parent), suit_(suit) {}

bool TargetPile::IsValid(const Card& card) {
  if (!card.isFaceUp()) {
    return false;
  }
  if (card.GetSuit() != suit_) {
    return false;
  }
  if (cards_.empty()) {
    return card.GetRank() == Rank::ACE;
  } else {
    return card.GetRank() == cards_.back()->GetRank() + 1;
  }
}

// TODO:
void TargetPile::mousePressEvent(QGraphicsSceneMouseEvent* event) {}