#include "targetPile.hpp"

#include <QDebug>

TargetPile::TargetPile(QGraphicsItem *parent) : Pile(parent) {
  qDebug() << "Created TargetPile";
}

bool TargetPile::IsValid(const Card &card) {
  if (!card.isFaceUp()) {
    return false;
  }
  if (this->Size() == 0) {
    suit_ = card.GetSuit();
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
  Card *card = this->getTopCard();
  if (card != nullptr) {
    // Get the card's previous position
    QPointF prevPos = card->getPrevScenePos();
    QPointF startPos = this->mapFromScene(prevPos);

    // Set the start and end positions for the animation
    QPointF endPos = QPointF(0, 1);
    QPointF endScenePos = mapToScene(endPos);
    card->setPrevScenePos(endScenePos);

    // Start the animation
    if (card->pos() != endPos) {
      this->setZValue(5);
      card->animateMove(startPos, endPos);
    }
  }
}

void TargetPile::setCardPrevScenePos() {
  QPointF scenePos = mapToScene(QPointF(0, 1));
  for (auto &card : cards_) {
    card->setPrevScenePos(scenePos);
  }
}

void TargetPile::paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) {
  Q_UNUSED(option);
  Q_UNUSED(widget);

  QPainterPath path;

  // Set font
  QFont font("Arial", 100, QFont::Bold);
  QFontMetricsF metrics(font);

  // Text and its bounding box
  QString text = "A";
  QRectF textRect = metrics.boundingRect(text);

  // Center the text inside rect_
  QPointF center(
      rect_.center().x() - textRect.width() / 2 - 2,
      rect_.center().y() + textRect.height() / 2 - metrics.descent());

  // Add text to the path at the calculated position
  path.addText(center, font, text);

  // Set brush and pen
  painter->setBrush(QColor(0x52E457));  // Fill color
  painter->setPen(Qt::NoPen);           // No outline

  // Fill the text path
  painter->fillPath(path, painter->brush());

  // Optional: Draw the outer rectangle
  painter->setBrush(Qt::transparent);
  painter->setPen(Qt::black);
  painter->drawRect(rect_);
}