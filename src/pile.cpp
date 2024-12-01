#include "pile.hpp"

#include <QDebug>
#include <stack>

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

void Pile::TransferCard(Pile& other, size_t nof) {
  qDebug() << "TransferCard called with nof =" << nof;
  qDebug() << "Current pile size:" << this->Size();
  qDebug() << "Other pile size before transfer:" << other.Size();

  if (!this->Empty() && nof <= this->Size()) {
    if (nof == 1) {
      qDebug() << "Transferring one card";
      unique_ptr<Card> card = RemoveCard();
      other.AddCard(card);
    } else {
      qDebug() << "Transferring multiple cards";
      stack<unique_ptr<Card>> aux;
      size_t i = 0;
      while (i < nof && !cards_.empty()) {
        qDebug() << "Removing card" << i + 1;
        aux.push(RemoveCard());
        ++i;
      }
      while (!aux.empty()) {
        qDebug() << "Adding card to other pile";
        other.AddCard(aux.top());
        aux.pop();
      }
    }
  } else {
    qDebug() << "TransferCard conditions not met: Empty or nof > Size";
  }

  qDebug() << "Other pile size after transfer:" << other.Size();
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
