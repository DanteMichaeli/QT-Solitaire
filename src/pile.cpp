#include "pile.hpp"

#include <QDebug>

Pile::Pile(QGraphicsItem* parent)
    : QGraphicsObject(parent), rect_(0, 0, 100, 150) {}

Pile::~Pile() { qDebug() << "Pile destroyed."; }

Card* Pile::getTopCard() const {
  if (!this->Empty()) {
    return cards_.back().get();
  }
  throw std::out_of_range("pile empty");
}

void Pile::AddCard(std::unique_ptr<Card>& card) {
  card->setParentItem(this);
  card->setPos(0, 0);
  connect(card.get(), &Card::cardClicked, this, &Pile::onCardClicked);
  connect(card.get(), &Card::cardDragged, this, &Pile::onCardDragged);
  cards_.push_back(std::move(card));
}

unique_ptr<Card> Pile::RemoveCard() {
  if (cards_.empty()) {
    return nullptr;
  }
  unique_ptr<Card> cardPtr = std::move(cards_.back());
  cardPtr->setParentItem(nullptr);
  disconnect(cardPtr.get(), &Card::cardClicked, this, &Pile::onCardClicked);
  disconnect(cardPtr.get(), &Card::cardDragged, this, &Pile::onCardDragged);
  cards_.pop_back();
  return cardPtr;
}

bool Pile::TransferCard(Pile& other) {
  if (cards_.empty()) {
    return false;
  }
  if (!other.IsValid(*cards_.back())) {
    return false;
  }
  unique_ptr<Card> card = RemoveCard();
  other.AddCard(card);
  updateVisuals();
  other.updateVisuals();

  return true;
}

QRectF Pile::boundingRect() const { return rect_; }

void Pile::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                 QWidget* widget) {
  Q_UNUSED(option);
  Q_UNUSED(widget);
  painter->setBrush(Qt::lightGray);
  painter->drawRect(rect_);
}

void Pile::onCardClicked(Card* card) {}
void Pile::onCardDragged(Card* card, const QPointF& newPosition) {}
