#include "klondikePile.hpp"

KlondikePile::KlondikePile(Deck& deck, size_t nofCards, size_t nofFacingUp,
                           QGraphicsItem* parent)
    : Pile(parent) {
  qDebug() << "Creating KlondikePile with" << nofCards << "cards.";
  for (size_t i = 0; i < nofCards; i++) {
    auto card = deck.dealCard();
    if (!card) {
      qDebug() << "Error: Attempted to deal a card, but the deck is empty!";
      throw std::runtime_error("Deck is empty during pile creation.");
    }
    if (i >= nofCards - nofFacingUp) {
      card->flipUp();
    }
    AddCard(card);
    updateVisuals();  // Initialize the visual representation
  }
  qDebug() << "KlondikePile created with" << cards_.size() << "cards.";
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

bool KlondikePile::TransferSubPile(KlondikePile& other, size_t nofCards) {
  if (nofCards > cards_.size()) {
    return false;
  }
  size_t start = cards_.size() - nofCards;
  if (!other.IsValid(*cards_[start])) {
    return false;
  }
  for (size_t i = start; i < cards_.size(); i++) {
    other.AddCard(cards_[i]);
  }
  for (size_t j = 0; j < nofCards; j++) {
    RemoveCard();
  }
  return true;
}

unique_ptr<Card> KlondikePile::RemoveCard() {
  auto cardPtr = Pile::RemoveCard();
  if (cardPtr != nullptr) {
    cards_.back()->flipUp();
  }
  return cardPtr;
}
// TODO:
void KlondikePile::mousePressEvent(QGraphicsSceneMouseEvent* event) {}