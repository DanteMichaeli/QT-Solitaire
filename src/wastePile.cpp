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
  int show = 3;
  int index = Size();
  int i = std::max(Size() - show, 0);
  while (index > 0) {
    index--;
    // Get the card's previous position
    Card* card = cards_[index].get();
    QPointF prevPos = card->getPrevScenePos();
    QPointF startPos = this->mapFromScene(prevPos);

    // Set the start and end positions for the animation
    int j = (index >= i) ? index - i : 0;
    QPointF endPos = QPointF(j * PILE_YOFFSET, 1);
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
void WastePile::setCardPrevScenePos() {
  int show = 3;
  int i = std::max(Size() - show, 0);
  for (int index = 0; index < Size(); index++) {
    int j = (index >= i) ? index - i : 0;
    QPointF scenePos = mapToScene(QPointF(j * PILE_YOFFSET, 1));
    cards_[index]->setPrevScenePos(scenePos);
  }
}
