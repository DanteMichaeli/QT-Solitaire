#include "pile.hpp"

#include <QDebug>
#include <stack>

Pile::Pile(QGraphicsItem* parent)
    : QGraphicsObject(parent), rect_(0, 0, 100, 150) {}

Pile::~Pile() { qDebug() << "PILE destroyed"; }

Card* Pile::getTopCard() const {
  if (!this->Empty()) {
    return cards_.back();
  }
  return nullptr;
}

void Pile::AddCard(Card* card) {
  card->setParentItem(this);
  connect(card, &Card::cardClicked, this, &Pile::onCardClicked);
  connect(card, &Card::cardDragged, this, &Pile::onCardDragged);
  cards_.push_back(card);
}

Card* Pile::RemoveCard() {
  if (cards_.empty()) {
    return nullptr;
  }
  Card* card = getTopCard();
  card->setParentItem(nullptr);
  disconnect(card, &Card::cardClicked, this, &Pile::onCardClicked);
  disconnect(card, &Card::cardDragged, this, &Pile::onCardDragged);
  cards_.pop_back();
  return card;
}

Card* Pile::getCardFromBack(size_t i) {
  int index = cards_.size() - 1 - i;
  if (index >= 0 && index < cards_.size()) {
    return cards_[index];
  }
  return nullptr;
}

void Pile::TransferCard(Pile& other, size_t nof) {
  if (!this->Empty() && nof <= this->Size()) {
    if (nof == 1) {
      Card* card = RemoveCard();
      other.AddCard(card);
    } else {
      stack<Card*> aux;
      size_t i = 0;
      while (i < nof && !cards_.empty()) {
        aux.push(RemoveCard());
        ++i;
      }
      while (!aux.empty()) {
        auto& card = aux.top();
        other.AddCard(card);
        aux.pop();
      }
    }
  }
}

vector<Card*> Pile::getCardsAbove(Card* card) {
  vector<Card*> above;
  bool found = false;
  for (auto& cardPtr : cards_) {
    if (!found && cardPtr == card) {
      found = true;
    }
    if (found) {
      above.push_back(cardPtr);
    }
  }
  return above;
}

int Pile::cardIndexFromBack(Card* card) const {
  auto backIt = cards_.end();
  int i = 0;
  while (backIt != cards_.begin()) {
    i++;
    backIt--;
    if (*backIt == card) {
      return i;
    }
  }
  return 0;
}

QRectF Pile::boundingRect() const { return rect_; }

void Pile::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                 QWidget* widget) {
  Q_UNUSED(option);
  Q_UNUSED(widget);
  painter->setBrush(Qt::transparent);
  painter->setPen(Qt::darkGreen);
  painter->drawRect(rect_);
}

void Pile::onCardClicked(Card* card) { emit cardClickMove(card, this); }
void Pile::onCardDragged(Card* card, const QPointF& newScenePos) {
  emit cardMoved(card, this, newScenePos);
}
