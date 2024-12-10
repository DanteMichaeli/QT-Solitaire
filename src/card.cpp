#include "card.hpp"

#include <QDebug>
#include <QGraphicsScene>
#include <QSize>

#include "deck.hpp"
#include "klondikePile.hpp"
#include "targetPile.hpp"
#include "wastePile.hpp"

Card::Card(Suit s, Rank r, QGraphicsItem *parent)
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
  glowEffect_->setBlurRadius(0);
  glowEffect_->setColor(Qt::darkRed);
  glowEffect_->setOffset(0, 0);
  setGraphicsEffect(glowEffect_);

  // Glow-in animation
  glowIn_ = new QPropertyAnimation(this, "glowRadius");
  glowIn_->setDuration(500);
  glowIn_->setStartValue(0);
  glowIn_->setEndValue(110);
  glowIn_->setEasingCurve(QEasingCurve::OutQuad);

  glowOut_ = new QPropertyAnimation(this, "glowRadius");
  glowOut_->setDuration(500);
  glowOut_->setStartValue(110);
  glowOut_->setEndValue(0);
  glowOut_->setEasingCurve(QEasingCurve::InQuad);

  glowTimer_ = new QTimer(this);
  glowTimer_->setSingleShot(true);

  isGlowing_ = false;

  // Moving animation
  animation_ = new QPropertyAnimation(this, "pos");
  animation_->setDuration(500);
  animation_->setEasingCurve(QEasingCurve::InOutQuad);
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
    default:
      return "";
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

/*
Card is clickabe if:

- It is faceUp and in klondike pile
- It is part of deck
- It is top card of wastepile
*/
bool Card::isClickable() {
  Pile *pile = static_cast<Pile *>(this->parentItem());
  if (dynamic_cast<KlondikePile *>(pile) != nullptr) {
    return isFaceUp();
  }
  if (dynamic_cast<WastePile *>(pile) != nullptr) {
    return pile->getTopCard() == this;
  }
  return true;
}

/*
Card is draggable if:

- Not in deck and clickable
*/
bool Card::isDraggable() {
  if (dynamic_cast<Deck *>(this->parentItem()) != nullptr) {
    return false;
  }
  return isClickable();
}

bool Card::operator==(Card &card) {
  return suit_ == card.GetSuit() && rank_ == card.GetRank();
}

QRectF Card::boundingRect() const {
  return QRectF(0, 0, pixmap_.width(), pixmap_.height());
}

void Card::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                 QWidget *widget) {
  Q_UNUSED(option);
  Q_UNUSED(widget);
  pixmap_ = this->isFaceUp() ? frontImage_ : backImage_;
  painter->drawPixmap(0, 0, pixmap_);
}

void Card::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  if (animation_->state() != QAbstractAnimation::Running) {
    if (event->button() == Qt::LeftButton && this->isClickable()) {
      if (isGlowing_) {
        stopGlowingAnimation();
      }
      prevPos_ = this->pos();
      auto parent = this->parentItem();
      parent->setZValue(5);
      auto kPile = dynamic_cast<KlondikePile *>(parent);
      if (kPile != nullptr) {
        cardsAbove_ = kPile->getCardsAbove(this);
      } else {
        cardsAbove_.push_back(this);
      }
    }
  }
  QGraphicsItem::mousePressEvent(event);
}

void Card::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  if (animation_->state() != QAbstractAnimation::Running) {
    if (this->isDraggable() &&
        (event->pos() - prevPos_).manhattanLength() > 0) {
      // Map the scene delta to the parent's local coordinate system
      QGraphicsItem *parentItem = this->parentItem();
      QPointF parentDelta = parentItem->mapFromScene(event->scenePos()) -
                            parentItem->mapFromScene(event->lastScenePos());

      for (auto &card : cardsAbove_) {
        card->setPos(card->pos() + parentDelta);
      }
    }
  }
}

void Card::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
  auto parent = this->parentItem();
  if (glowIn_->state() != QAbstractAnimation::Running) {
    if (event->button() == Qt::LeftButton && this->isClickable()) {
      int dist = (prevPos_ - this->pos()).manhattanLength();
      if (dist == 0 && dynamic_cast<TargetPile *>(parent) == nullptr) {
        emit cardClicked(this);
      } else {
        for (auto &card : cardsAbove_) {
          auto prevPos = card->scenePos();
          card->setPrevScenePos(prevPos);
        }
        if (dist >= 10) {
          emit cardDragged(this, event->scenePos());
        } else {
          static_cast<Pile *>(parent)->updateVisuals();
        }
      }
    }
  }
  parent->setZValue(1);
  cardsAbove_.clear();
  QGraphicsItem::mouseReleaseEvent(event);
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
  for (auto &card : cardsAbove_) {
    auto &pxm = card->getPixmap();
    painter.drawPixmap(0, currentY, pxm);
    currentY += offset;
  }
  painter.end();
  tmpDragMap_ = combinedPixmap;
}

void Card::startGlowing() {
  if (isGlowing_) {
    return;
  }
  isGlowing_ = true;

  connect(glowTimer_, &QTimer::timeout, this, &Card::startGlowingOut);
  glowTimer_->start(1000);
  glowIn_->start();
}

void Card::startGlowingOut() {
  // Glow-out animation
  connect(glowOut_, &QPropertyAnimation::finished, this,
          [this]() { isGlowing_ = false; });
  glowOut_->start();
}

void Card::stopGlowingAnimation() {
  QVariant glow = 110;
  if (glowIn_->state() == QAbstractAnimation::Running) {
    glow = glowIn_->currentValue();
  } else if (glowOut_->state() == QAbstractAnimation::Running) {
    glow = glowOut_->currentValue();
  }

  QPropertyAnimation *fastGlowOut = new QPropertyAnimation(this, "glowRadius");
  fastGlowOut->setStartValue(glow);
  fastGlowOut->setEndValue(0);
  fastGlowOut->setDuration(75);
  connect(fastGlowOut, &QPropertyAnimation::finished, this,
          [this]() { isGlowing_ = false; });
  fastGlowOut->start(QAbstractAnimation::DeleteWhenStopped);
  glowIn_->stop();
  glowOut_->stop();
  glowTimer_->stop();
}

void Card::animateMove(QPointF &startPos, QPointF &endPos) {
  QGraphicsItem *parent = this->parentItem();
  animation_->setStartValue(startPos);
  animation_->setEndValue(endPos);
  connect(animation_, &QAbstractAnimation::finished, this,
          [this, parent]() { parent->setZValue(1); });
  animation_->start();
}
