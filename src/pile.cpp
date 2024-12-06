#include "pile.hpp"

#include <QDebug>
#include <stack>

Pile::Pile(QGraphicsItem* parent)
    : QGraphicsObject(parent), rect_(0, 0, 100, 150) {}

Pile::~Pile() {}

Card* Pile::getTopCard() const {
  if (!this->Empty()) {
    return cards_.back().get();
  }
  return nullptr;
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

Card* Pile::getCardFromBack(size_t i) {
  int index = cards_.size() - 1 - i;
  if (index >= 0 && index < cards_.size()) {
    return cards_[index].get();
  }
  return nullptr;
}

void Pile::TransferCard(Pile& other, size_t nof) {
  if (!this->Empty() && nof <= this->Size()) {
    if (nof == 1) {
      unique_ptr<Card> card = RemoveCard();
      other.AddCard(card);
    } else {
      stack<unique_ptr<Card>> aux;
      size_t i = 0;
      while (i < nof && !cards_.empty()) {
        aux.push(RemoveCard());
        ++i;
      }
      while (!aux.empty()) {
        other.AddCard(aux.top());
        aux.pop();
      }
    }
  }
}

vector<Card*> Pile::getCardsAbove(Card* card) {
  vector<Card*> above;
  bool found = false;
  for (auto& cardPtr : cards_) {
    if (!found && cardPtr.get() == card) {
      found = true;
    }
    if (found) {
      above.push_back(cardPtr.get());
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
    if (backIt->get() == card) {
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
  painter->setBrush(Qt::lightGray);
  painter->drawRect(rect_);
}

void Pile::onCardClicked(Card* card) { emit cardClickMove(card, this); }
void Pile::onCardDragged(Card* card, const QPointF& newScenePos) {
  emit cardMoved(card, this, newScenePos);
}
