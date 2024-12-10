#include "klondikePile.hpp"

KlondikePile::KlondikePile(QGraphicsItem* parent) : Pile(parent) {}

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
  int i = Size();
  while (i > 0) {
    i--;
    // Get the card's previous position
    Card* card = cards_[i].get();
    QPointF prevPos = card->getPrevScenePos();
    QPointF startPos = this->mapFromScene(prevPos);

    // Set the start and end positions for the animation
    QPointF endPos(0, 1 + i * PILE_YOFFSET);  // Offset for stacking
    QPointF endScenePos = mapToScene(endPos);
    card->setPrevScenePos(endScenePos);

    // Start the animation
    if (card->pos() != endPos) {
      this->setZValue(5);
      card->animateMove(startPos, endPos);
    } else {
      break;
    }
  }
}

void KlondikePile::setCardPrevScenePos() {
  for (int i = 0; i < Size(); i++) {
    QPointF scenePos = mapToScene(QPointF(0, 1 + i * PILE_YOFFSET));
    cards_[i]->setPrevScenePos(scenePos);
  }
}

QRectF KlondikePile::boundingRect() const { return QRect(-10, -10, 120, 500); }