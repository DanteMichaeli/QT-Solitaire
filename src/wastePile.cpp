#include "wastePile.hpp"

#include <QDebug>

#include "deck.hpp"

WastePile::WastePile(QGraphicsItem* parent) : Pile(parent) {}

// LOGIC RELATED FUNCTIONS

bool WastePile::isValid(const Card& card) { return false; }

int WastePile::addFromDeck(Deck& deck, const unsigned int nofCards) {
  size_t i = 0;
  while (i < nofCards && !deck.isEmpty()) {
    Card* topCard = deck.getTopCard();
    if (!topCard->isFaceUp()) {
      topCard->flip();
    }

    deck.transferCards(*this);
    i++;
  }
  return i;
}

void WastePile::undoAddFromDeck(Deck& deck, const unsigned int nofCards) {
  size_t i = 0;
  while (i < nofCards && !this->isEmpty()) {
    Card* topCard = getTopCard();
    if (topCard->isFaceUp()) {
      topCard->flip();
    }
    transferCards(deck);
    i++;
  }
}

// GUI RELATED FUNCTIONS

void WastePile::updateVisuals() {
  int index = getSize();
  int i = std::max(index - 3, 0);
  while (index > 0) {
    index--;
    // Get the card's previous position
    Card* card = cards_[index];
    QPointF prevPos = card->getPrevScenePos();
    QPointF startPos = this->mapFromScene(prevPos);

    // Set the end positions for the animation
    int j = (index >= i) ? index - i : 0;
    QPointF endPos = j * this->getOffset();

    // Start the animation
    if (startPos != endPos) {
      this->setZValue(2);
      card->animateMove(startPos, endPos);
    } else {
      break;
    }
  }
}
void WastePile::setCardsPrevScenePos() {
  int i = this->getSize() - 3;
  i = std::max(i, 0);

  for (int index = 0; index < this->getSize(); index++) {
    int j = (index >= i) ? index - i : 0;
    QPointF scenePos = mapToScene(j * this->getOffset());
    cards_[index]->setPrevScenePos(scenePos);
  }
}

QPointF WastePile::getOffset() const { return QPoint(PILE_OFFSET, 0); }
