#include "deck.hpp"

#include <QDebug>

Deck::Deck(QGraphicsItem* parent) : QGraphicsPixmapItem(parent) {
  // Add cards to deck
  for (Suit suit : allSuits) {
    for (Rank rank : allRanks) {
      cards_.push_back(std::make_unique<Card>(suit, rank));
    }
  }
  // Load the deck image (face-down)
  QString frontImagePath = ":/cards/face_down.png";
  if (!frontImage_.load(frontImagePath)) {
    qDebug() << "Failed to load deck image:" << frontImagePath;
  }
  setPixmap(frontImage_);
}

void Deck::mousePressEvent(QGraphicsSceneMouseEvent* event) {
  if (!Empty()) {
    Shuffle();
    qDebug() << "Deck shuffled.";
  } else {
    qDebug() << "Deck is empty.";
  }

  // Call the base class implementation (optional, if you want default behavior)
  QGraphicsPixmapItem::mousePressEvent(event);
}
