#include "targetPile.hpp"

#include <QDebug>

TargetPile::TargetPile(QGraphicsItem *parent) : Pile(parent) {
  qDebug() << "Created TargetPile";
}

// LOGIC RELATED FUNCTIONS

bool TargetPile::isValid(const Card &card) {
  if (!card.isFaceUp()) {
    return false;
  }
  if (this->isEmpty()) {
    // Dynamically adjust suit
    if (card.getRank() == Rank::ACE) {
      suit_ = card.getSuit();
      return true;
    } else {
      return false;
    }
  }
  if (card.getSuit() != suit_) {
    return false;
  }
  if (card.getRank() != cards_.back()->getRank() + 1) {
    return false;
  }
  return true;
}

// GUI RELATED FUNCTIONS

void TargetPile::updateVisuals() {
  Card *card = this->getTopCard();
  if (card != nullptr) {
    // Get the card's previous position
    QPointF prevPos = card->getPrevScenePos();
    QPointF startPos = this->mapFromScene(prevPos);

    // Set the start and end positions for the animation
    QPointF endPos = QPointF(0, 0);

    // Start the animation
    if (startPos != endPos) {
      this->setZValue(2);
      card->animateMove(startPos, endPos);
    }
  }
}

void TargetPile::setCardsPrevScenePos() {
  QPointF scenePos = mapToScene(this->getOffset());
  for (auto &card : cards_) {
    card->setPrevScenePos(scenePos);
  }
}

QPointF TargetPile::getOffset() const { return QPoint(0, 0); }

void TargetPile::paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) {
  Q_UNUSED(option);
  Q_UNUSED(widget);

  QPainterPath path;

  // Set font
  QFont font("Arial", 100, QFont::Bold);
  QFontMetricsF metrics(font);

  // Text and its bounding box centered in the rectangle
  QString text = "A";
  QRectF textRect = metrics.boundingRect(text);

  QPointF center(
      rect_.center().x() - textRect.width() / 2 - 2,
      rect_.center().y() + textRect.height() / 2 - metrics.descent());

  // Add text to the path at the calculated position
  path.addText(center, font, text);

  // Set brush and pen
  painter->setBrush(QColor(0x52E457));
  painter->setPen(Qt::NoPen);  // No outline

  // Fill the text path
  painter->fillPath(path, painter->brush());

  // Draw the outer rectangle
  painter->setBrush(Qt::transparent);
  painter->setPen(Qt::black);
  painter->drawRect(rect_);
}