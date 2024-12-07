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
  // Render only top 2 cards (potentially for smooth animation)
  int size = Size();
  if (size == 0) {
    return;
  } else if (size <= 2) {
    cards_[size - 1]->setPos(0, 0);
  } else {
    cards_[size - 1]->setPos(0, 0);
    cards_[size - 3]->hide();
  }
}

void TargetPile::paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) {
  Q_UNUSED(option);
  Q_UNUSED(widget);
  /*
     QPainterPath path;

     QFont font("Arial", 100, QFont::Bold);
     QPointF center(rect_.width() / 4, rect_.height());
     path.addText(center, font, "A");

     // Set the pen for borders
     painter->setBrush(QColor(0x52E457));  // Fill color
     painter->setPen(Qt::NoPen);           // No outline (optional)

     // Fill the text path
     painter->fillPath(path, painter->brush());

     painter->setBrush(Qt::transparent);
     painter->setPen(Qt::black);
     painter->drawRect(rect_);
     */
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