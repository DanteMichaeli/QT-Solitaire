#include "klondikePile.hpp"

KlondikePile::KlondikePile(Deck& deck, size_t nofCards, size_t nofFacingUp,
                           QGraphicsItem* parent)
    : Pile(parent) {
  qDebug() << "Creating KlondikePile with" << nofCards << "cards.";
  for (size_t i = 0; i < nofCards; i++) {
    auto card = deck.getTopCard();
    if (!card) {
      qDebug() << "Error: Attempted to deal a card, but the deck is empty!";
      throw std::runtime_error("Deck is empty during pile creation.");
    }
    if (i >= nofCards - nofFacingUp) {
      card->flipUp();
    }
    deck.TransferCard(*this);
  }
  updateVisuals();
  qDebug() << "KlondikePile created with" << cards_.size() << "cards.";
}

bool KlondikePile::flipCard(bool faceUp, int indexFromBack) {
  if (!this->Empty()) {
    int size = this->Size();
    Card* card = cards_[size - indexFromBack].get();
    if (!faceUp) {
      card->flipDown();
    } else if (!card->isFaceUp()) {
      card->flipUp();
      return true;
    }
  }
  return false;
}

bool KlondikePile::IsValid(const Card& card) {
  if (!card.isFaceUp()) {
    return false;
  }
  if (cards_.empty()) {
    return card.GetRank() == Rank::KING;
  }
  Card* top = cards_.back().get();
  bool diffColor = card.GetColor() != top->GetColor();
  bool isNextLower = card.GetRank() == top->GetRank() - 1;
  return diffColor && isNextLower;
}

void KlondikePile::updateVisuals() {
  int i = 0;
  for (auto card = cards_.begin(); card != cards_.end(); card++, i++) {
    (*card)->setPos(0, i * PILE_YOFFSET);
  }
}

QRectF KlondikePile::boundingRect() const { return QRect(-10, -10, 120, 500); }