#include "klondikePile.hpp"

KlondikePile::KlondikePile(QGraphicsItem* parent) : Pile(parent) {}

// LOGIC RELATED FUNCTIONS

bool KlondikePile::isValid(const Card& card) {
  if (!card.isFaceUp()) {
    return false;
  }
  if (this->isEmpty()) {
    return card.getRank() == Rank::KING;
  }
  Card* top = getTopCard();
  bool diffColor = card.getColor() != top->getColor();
  bool isNextLower = card.getRank() == top->getRank() - 1;
  return diffColor && isNextLower;
}

// GUI RELATED FUNCTIONS

void KlondikePile::updateVisuals() {
  int i = this->getSize();
  while (i > 0) {
    i--;
    // Get the card's previous position
    Card* card = cards_[i];
    QPointF prevPos = card->getPrevScenePos();
    QPointF startPos = this->mapFromScene(prevPos);

    // Set the start and end positions for the animation
    QPointF endPos = i * this->getOffset();  // Offset for stacking

    // Start the animation
    if (startPos != endPos) {
      this->setZValue(2);
      card->animateMove(startPos, endPos);
    } else {
      break;
    }
  }
}

void KlondikePile::setCardsPrevScenePos() {
  for (int i = 0; i < this->getSize(); i++) {
    QPointF scenePos = mapToScene(i * this->getOffset());
    cards_[i]->setPrevScenePos(scenePos);
  }
}

QPointF KlondikePile::getOffset() const {
  int offset = (getSize() > 11) ? 11 * PILE_OFFSET / getSize() : PILE_OFFSET;
  return QPoint(0, offset);
}

QRectF KlondikePile::boundingRect() const { return QRect(-10, -10, 120, 500); }