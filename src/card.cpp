#include "card.hpp"

#include <QDebug>
#include <QGraphicsScene>
#include <QSize>

#include "deck.hpp"
#include "klondikePile.hpp"
#include "targetPile.hpp"
#include "wastePile.hpp"

Card::Card(Suit s, Rank r, QGraphicsItem* parent)
    : suit_(s), rank_(r), faceUp_(false), QGraphicsObject(parent) {
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
  glowEffect_ = new QGraphicsDropShadowEffect(this);
  glowEffect_->setBlurRadius(0);  // No glow by default
  glowEffect_->setColor(Qt::darkRed);
  glowEffect_->setOffset(0, 0);
  setGraphicsEffect(glowEffect_);
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
  update();
}

void Card::flipDown() {
  faceUp_ = false;
  update();
}

void Card::toggleFace() {
  faceUp_ ^= true;
  update();
}

void Card::toggleFace() {
  faceUp_ ^= true;
  update();
}

/*
Card is clickabe if:

- It is faceUp and in klondike pile
- It is part of deck
- It is top card of wastepile
*/
bool Card::isClickable() {
  Pile* pile = static_cast<Pile*>(this->parentItem());
  if (dynamic_cast<KlondikePile*>(pile) != nullptr) {
    return isFaceUp();
  }
  if (dynamic_cast<WastePile*>(pile) != nullptr) {
    return pile->getTopCard() == this;
  }
  if (dynamic_cast<Deck*>(pile) != nullptr) {
    return true;
  }
  return false;
}

/*
Card is draggable if:

- Not in deck and clickable
*/
bool Card::isDraggable() {
  if (dynamic_cast<Deck*>(this->parentItem()) != nullptr) {
    return false;
  }
  return isClickable();
}

bool Card::operator==(Card& card) {
  return suit_ == card.GetSuit() && rank_ == card.GetRank();
}

QRectF Card::boundingRect() const {
  return QRectF(0, 0, pixmap_.width(), pixmap_.height());
}

void Card::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                 QWidget* widget) {
  Q_UNUSED(option);
  Q_UNUSED(widget);
  pixmap_ = this->isFaceUp() ? frontImage_ : backImage_;
  painter->drawPixmap(0, 0, pixmap_);
}

void Card::mousePressEvent(QGraphicsSceneMouseEvent* event) {
  if (event->button() == Qt::LeftButton && this->isClickable()) {
    prevPos_ = this->pos();
    auto parent = this->parentItem();
    parent->setZValue(parent->zValue() + 1);
    auto kPile = dynamic_cast<KlondikePile*>(parent);
    if (kPile != nullptr) {
      cardsAbove_ = kPile->getCardsAbove(this);
    }
  }
  QGraphicsItem::mousePressEvent(event);
}

void Card::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
  if (this->isDraggable() && (event->pos() - prevPos_).manhattanLength() > 0) {
    QPointF delta = event->scenePos() - event->lastScenePos();
    for (auto& card : cardsAbove_) {
      card->setPos(card->pos() + delta);
    }
  }
}

void Card::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
  cardsAbove_.clear();
  auto parent = this->parentItem();
  parent->setZValue(parent->zValue() - 1);

  if (event->button() == Qt::LeftButton && this->isClickable()) {
    QPointF newPos = this->pos();
    int dist = (newPos - prevPos_).manhattanLength();
    if (dist == 0) {
      emit cardClicked(this);
    } else if (dist >= 10) {
      emit cardDragged(this, event->scenePos());
    } else {
      static_cast<Pile*>(this->parentItem())->updateVisuals();
    }
    QGraphicsItem::mouseReleaseEvent(event);
  }
}

// Not working :( not in use currently
// Should combine all pixmaps of cardsAbove_ and update card "image" to it.
// Problem: pixmap width / height calculations cause some kind of error.
// Result is that combined pixmap clips some cards and leaves artifacts.
void Card::createDragPixmap() {
  int offset = 0;
  int totalWidth = pixmap_.width();
  int totalHeight = pixmap_.height() + (cardsAbove_.size() - 1) * offset;

  QPixmap combinedPixmap(totalWidth, totalHeight);
  combinedPixmap.fill(Qt::transparent);
  QPainter painter(&combinedPixmap);
  int currentY = 0;
  for (auto& card : cardsAbove_) {
    auto& pxm = card->getPixmap();
    painter.drawPixmap(0, currentY, pxm);
    currentY += offset;
  }
  painter.end();
  tmpDragMap_ = combinedPixmap;
}

void Card::startGlowing() {
  // Glow-in animation
  QPropertyAnimation* animationIn = new QPropertyAnimation(this, "glowRadius");
  animationIn->setDuration(500);
  animationIn->setStartValue(0);
  animationIn->setEndValue(110);
  animationIn->setEasingCurve(QEasingCurve::OutQuad);

  // Pause at maximum glow
  QTimer::singleShot(1000, this, [this]() { startGlowingOut(); });

  animationIn->start(QAbstractAnimation::DeleteWhenStopped);
}

void Card::startGlowingOut() {
  // Glow-out animation
  QPropertyAnimation* animationOut = new QPropertyAnimation(this, "glowRadius");
  animationOut->setDuration(500);
  animationOut->setStartValue(110);
  animationOut->setEndValue(0);
  animationOut->setEasingCurve(QEasingCurve::InQuad);

  animationOut->start(QAbstractAnimation::DeleteWhenStopped);
}
