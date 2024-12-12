#include "klondikePile.hpp"

KlondikePile::KlondikePile(QGraphicsItem* parent) : Pile(parent) {}

bool KlondikePile::flipCard(bool faceUp, int indexFromBack) {
  if (this->Empty()) {
    return false;  // No cards to flip
  }

  int size = this->Size();

  Card* card = cards_[size - indexFromBack];

  if (faceUp) {
    if (!card->isFaceUp()) {
      card->flip();  // Flip to face-up
      return true;   // Successfully flipped
    }
  } else {
    if (card->isFaceUp()) {
      card->flip();  // Flip to face-down
    }
  }

  return false;  // No action taken
}

bool KlondikePile::IsValid(const Card& card) {
  if (!card.isFaceUp()) {
    return false;
  }
  if (cards_.empty()) {
    return card.getRank() == Rank::KING;
  }
  Card* top = getTopCard();
  bool diffColor = card.getColor() != top->getColor();
  bool isNextLower = card.getRank() == top->getRank() - 1;
  return diffColor && isNextLower;
}

void KlondikePile::updateVisuals() {
  int i = Size();
  while (i > 0) {
    i--;
    // Get the card's previous position
    Card* card = cards_[i];
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