#include "card.hpp"

#include <QDebug>

Card::Card(Suit s, Rank r, QGraphicsItem* parent)
    : suit_(s), rank_(r), faceUp_(false), QGraphicsPixmapItem(parent) {
  // Set color
  color_ = (s == Suit::SPADES || s == Suit::CLUBS) ? Color::BLACK : Color::RED;

  // Enable selection and movement of the CardItem
  setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);

  // Load the front/back image of the card
  QString frontIMG_path = QString(":/cards/%1.png").arg(cardToQString());
  QString backIMG_path = QString(":/cards/face_down.png");

  if (!frontImage_.load(frontIMG_path)) {
    qDebug() << "Failed to load front image:" << frontIMG_path;
  }
  if (!backImage_.load(backIMG_path)) {
    qDebug() << "Failed to load back image:" << backIMG_path;
  }

  setScale(SCALING_FACTOR);
  updateCardImage();
}

Card::~Card() { std::cout << "Card destroyed" << std::endl; }

QString Card::getSuitQstring() const {
  switch (GetSuit()) {
    case Suit::CLUBS:
      return QString("clubs");
    case Suit::DIAMONDS:
      return QString("diamonds");
    case Suit::HEARTS:
      return QString("hearts");
    case Suit::SPADES:
      return QString("spades");
  }
}

QString Card::getRankQstring() const {
  Rank rank = GetRank();
  switch (rank) {
    case Rank::ACE:
      return QString("ace");
    case Rank::JACK:
      return QString("jack");
    case Rank::QUEEN:
      return QString("queen");
    case Rank::KING:
      return QString("king");
    default:
      return QString::number(rank);
  }
}

QString Card::cardToQString() const {
  return QString("%1_of_%2").arg(getRankQstring()).arg(getSuitQstring());
}

void Card::flipUp() {
  faceUp_ = true;
  updateCardImage();
}

void Card::flipDown() {
  faceUp_ = false;
  updateCardImage();
}

void Card::updateCardImage() {
  // Update the pixmap based on whether the card is face-up or face-down
  if (isFaceUp()) {
    setPixmap(frontImage_);
  } else {
    setPixmap(backImage_);
  }
}