#include "pile.hpp"

#include <QBrush>
#include <QDebug>
#include <QPen>

Pile::Pile(QGraphicsItem* parent) : QGraphicsRectItem(parent) {
  setRect(0, 0, 100, 150);  // Default rectangle size
  setPen(QPen(Qt::black));
  setBrush(QBrush(Qt::transparent));
}

void Pile::AddCard(std::unique_ptr<Card>& card) {
  card->setParentItem(this);
  cards_.push_back(std::move(card));
}

unique_ptr<Card> Pile::RemoveCard() {
  if (cards_.empty()) {
    return nullptr;
  }
  unique_ptr<Card> cardPtr = std::move(cards_.back());
  cards_.pop_back();
  return cardPtr;
}

bool Pile::TransferCard(Pile& other) {
  if (cards_.empty()) {
    return false;
  }
  if (!other.IsValid(*cards_.back())) {
    return false;
  }
  other.AddCard(cards_.back());
  RemoveCard();
  return true;
}

void Pile::updateVisuals() {
  int i = 0;
  for (auto card = cards_.begin(); card != cards_.end(); card++, i++) {
    (*card)->setPos(0, i * PILE_YOFFSET);
  }
}
