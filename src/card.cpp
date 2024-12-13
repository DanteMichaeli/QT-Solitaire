#include "card.hpp"

#include <QDebug>

#include "deck.hpp"
#include "klondikePile.hpp"
#include "targetPile.hpp"
#include "wastePile.hpp"

Card::Card(Suit s, Rank r, QGraphicsItem *parent)
    : suit_(s),
      rank_(r),
      faceUp_(false),
      isMoving_(false),
      isDragged_(false),
      isGlowing_(false),
      flipProgress_(180),
      QGraphicsObject(parent) {
  color_ = (s == Suit::SPADES || s == Suit::CLUBS) ? Color::BLACK : Color::RED;

  setScale(SCALING_FACTOR);

  setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);

  QString frontIMG_path = QString(":/cards/%1.png").arg(cardToQString());
  QString backIMG_path = QString(":/cards/face_down.png");
  if (!frontImage_.load(frontIMG_path)) {
    qDebug() << "Failed to load front image:" << frontIMG_path;
  }
  if (!backImage_.load(backIMG_path)) {
    qDebug() << "Failed to load back image:" << backIMG_path;
  }
  // Glow effect
  glowEffect_ = new QGraphicsDropShadowEffect(this);
  glowEffect_->setBlurRadius(0);
  glowEffect_->setColor(Qt::darkRed);
  glowEffect_->setOffset(0, 0);
  setGraphicsEffect(glowEffect_);

  // Glow in animation
  glowInAnimation_ = new QPropertyAnimation(this, "getGlowRadius");
  glowInAnimation_->setDuration(500);
  glowInAnimation_->setStartValue(0);
  glowInAnimation_->setEndValue(MAX_GLOW);
  glowInAnimation_->setEasingCurve(QEasingCurve::OutQuad);

  // Glow out animation
  glowOutAnimation_ = new QPropertyAnimation(this, "getGlowRadius");
  glowOutAnimation_->setDuration(500);
  glowOutAnimation_->setStartValue(MAX_GLOW);
  glowOutAnimation_->setEndValue(0);
  glowOutAnimation_->setEasingCurve(QEasingCurve::InQuad);
  glowTimer_ = new QTimer(this);
  glowTimer_->setSingleShot(true);
  isGlowing_ = false;

  // Moving animation
  moveAnimation_ = new QPropertyAnimation(this, "movePos");
  moveAnimation_->setEasingCurve(QEasingCurve::InOutQuad);

  // Flip animation
  flipAnimation_ = new QPropertyAnimation(this, "getFlipProgress");
  flipAnimation_->setDuration(250);
  flipAnimation_->setStartValue(0);
  flipAnimation_->setEndValue(180);
  flipAnimation_->setEasingCurve(QEasingCurve::InOutCubic);
}

Card::~Card() { qDebug() << "Card: " << this->cardToQString() << "destroyed."; }

// LOGIC RELATED FUNCTIONS

Pile *Card::getPile() { return static_cast<Pile *>(this->parentItem()); }

QString Card::getSuitQstring() const {
  switch (getSuit()) {
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
  Rank rank = getRank();
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

void Card::flip() {
  faceUp_ = !faceUp_;
  animateFlip();
}
// Card is clickabe if it is from deck, face up in klondike, top card of waste /
// target pile
bool Card::isClickable() {
  Pile *pile = this->getPile();
  if (dynamic_cast<KlondikePile *>(pile)) {
    return isFaceUp();
  }
  if (dynamic_cast<WastePile *>(pile) || dynamic_cast<TargetPile *>(pile)) {
    return pile->getTopCard() == this;
  }
  return true;
}

// Card is draggable if it isn't in deck and is clickable
bool Card::isDraggable() {
  if (dynamic_cast<Deck *>(this->parentItem()) != nullptr) {
    return false;
  }
  return isClickable();
}

// GUI RELATED FUNCTIONS

void Card::setMovePos(const QPointF &pos) {
  prevScenePos_ = this->parentItem()->mapToScene(pos);
  this->setPos(pos);
}

void Card::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  // Is clickable and is not mid animation
  if (event->button() == Qt::LeftButton && this->isClickable() && !isMoving()) {
    // Bring up
    this->setParentZValue(1);
    if (isGlowing_) stopGlowingAnimation();  // Stop glow
    prevPos_ = this->pos();

    // Get all the cards above it in a pile
    cardsAbove_ = this->getPile()->getCardsAbove(this);

    // Set that the cards are being dragged
    for (auto &card : cardsAbove_) card->isDragged_ = true;
  }
}

void Card::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  if (this->isDraggable() && !isMoving()) {
    // Map the scene delta to the parent's local coordinate system
    Pile *parent = this->getPile();
    QPointF moveDelta = parent->mapFromScene(event->scenePos()) -
                        parent->mapFromScene(event->lastScenePos());

    // Move all the cards above
    int i = 0;
    for (auto &card : cardsAbove_) {
      // If a card in the "sub pile" is mid animation, adjust the animation to
      // end in the correct position.
      if (card->isMoving()) {
        card->moveAnimation_->setEndValue(this->pos() +
                                          i * parent->getOffset());
      } else {
        card->setPos(card->pos() + moveDelta);
      }
      i++;
    }
  }
}

void Card::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
  if (event->button() == Qt::LeftButton && this->isClickable() && !isMoving()) {
    // Set that the cards above are not being dragged
    // Also set the previous scenePos.
    for (auto &card : cardsAbove_) {
      card->setPrevScenePos(card->scenePos());
      card->isDragged_ = false;
    }
    // Bring back to normal z level
    Pile *parent = this->getPile();
    this->setParentZValue(0);
    int dist = (prevPos_ - this->pos()).manhattanLength();

    // Calculate the drag distance
    // If 0, trigger auto move
    // If over 10, trigger card dragged
    if (dist == 0 && !dynamic_cast<TargetPile *>(parent)) {
      emit cardClicked(this);
    } else if (dist >= 15) {
      emit cardDragged(this, event->scenePos());
    } else {
      parent->updateVisuals();
    }
    // Clear the sub pile
    cardsAbove_.clear();
  }
}

void Card::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                 QWidget *widget) {
  Q_UNUSED(option);
  Q_UNUSED(widget);

  int flipProg = getFlipProgress();
  pixmap_ = (this->isFaceUp() == (flipProg >= 90)) ? frontImage_ : backImage_;
  painter->drawPixmap(0, 0, pixmap_);
}

void Card::animateFlip() { flipAnimation_->start(); }

void Card::setFlipProgress(const qreal &progress) {
  flipProgress_ = progress;

  // Transform card to scale in X direction.
  // Illusion of flip
  QTransform transform;
  double scale = progress / 90.0;
  double scaleHor = (progress < 90) ? 1 - scale : scale - 1;

  QPointF center = this->boundingRect().center();
  double scaledX = center.x() * this->scale();

  transform.translate(scaledX, 1);
  transform.scale(scaleHor, 1.0);
  transform.translate(-scaledX, 1);
  this->setTransform(transform);
}

void Card::animateMove(const QPointF &startPos, const QPointF &endPos,
                       const unsigned int ms) {
  moveAnimation_->stop();
  isMoving_ = true;

  // Pile animates the card
  // startPos is determined by prevScenePoss_ more or less
  moveAnimation_->setDuration(ms);
  moveAnimation_->setStartValue(startPos);
  moveAnimation_->setEndValue(endPos);

  connect(moveAnimation_, &QAbstractAnimation::finished, this,
          [this, endPos]() {
            // Dont bring the Z value down if the card animation did't finish
            // And a new drag action was started
            if (!isDragged_) this->setParentZValue(0);
            isMoving_ = false;
            prevScenePos_ = this->parentItem()->mapToScene(endPos);
          });

  moveAnimation_->start();
}

void Card::animateGlow() {
  if (isGlowing_) {
    return;
  }
  isGlowing_ = true;

  // glow in for 500ms then hold there 500ms and then start glowOut
  connect(glowTimer_, &QTimer::timeout, this, &Card::animateGlowOut);
  glowTimer_->start(1000);
  glowInAnimation_->start();
}

void Card::animateGlowOut() {
  connect(glowOutAnimation_, &QPropertyAnimation::finished, this, [this]() {
    isGlowing_ = false;
    glowTimer_->stop();
  });
  glowOutAnimation_->start();
}

void Card::stopGlowingAnimation() {
  // Get the current glow value and start fast glowout from there
  qreal glow = std::min(glowInAnimation_->currentValue().toReal(),
                        glowOutAnimation_->currentValue().toReal());
  glowInAnimation_->stop();
  glowOutAnimation_->stop();
  glowTimer_->stop();

  auto fastGlowout = new QPropertyAnimation(this, "getGlowRadius");
  fastGlowout->setDuration(75);
  fastGlowout->setStartValue(glow);
  fastGlowout->setEndValue(0);
  fastGlowout->start(QAbstractAnimation::DeleteWhenStopped);
  isGlowing_ = false;
}

void Card::setParentZValue(const qreal &value) {
  this->parentItem()->setZValue(value);
}

QRectF Card::boundingRect() const {
  return QRectF(0, 0, pixmap_.width(), pixmap_.height());
}
