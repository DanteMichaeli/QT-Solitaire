#include "pile.hpp"

#include <QDebug>
#include <stack>

Pile::Pile(QGraphicsItem* parent)
    : QGraphicsObject(parent), rect_(0, 0, 100, 150) {}

Pile::~Pile() { qDebug() << "PILE destroyed"; }

// LOGIC RELATED FUNCTIONS

Card* Pile::getTopCard() const {
  if (!this->isEmpty()) {
    return cards_.back();
  }
  return nullptr;
}

void Pile::addCard(Card* card) {
  card->setParentItem(this);
  connect(card, &Card::cardClicked, this, &Pile::onCardClicked);
  connect(card, &Card::cardDragged, this, &Pile::onCardDragged);
  cards_.push_back(card);
}

Card* Pile::removeCard() {
  if (this->isEmpty()) {
    return nullptr;
  }
  Card* card = getTopCard();
  card->setParentItem(nullptr);
  disconnect(card, &Card::cardClicked, this, &Pile::onCardClicked);
  disconnect(card, &Card::cardDragged, this, &Pile::onCardDragged);
  cards_.pop_back();
  return card;
}

Card* Pile::getCardFromBack(const size_t i) {
  int index = getSize() - 1 - i;
  if (index >= 0 && index < cards_.size()) {
    return cards_[index];
  }
  return nullptr;
}

void Pile::transferCards(Pile& other, const unsigned int nof) {
  if (!this->isEmpty() && nof <= this->getSize()) {
    if (nof == 1) {
      Card* card = removeCard();
      other.addCard(card);
    } else {
      // aux stack to keep the order consistent
      stack<Card*> aux;
      size_t i = 0;
      while (i < nof && !cards_.empty()) {
        aux.push(this->removeCard());
        ++i;
      }
      while (!aux.empty()) {
        auto& card = aux.top();
        other.addCard(card);
        aux.pop();
      }
    }
  }
}

vector<Card*> Pile::getCardsAbove(Card* card) const {
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

bool Pile::flipTopCard(bool faceUp, const int indexFromBack) {
  if (this->isEmpty()) {
    return false;  // No cards to flip
  }

  int size = this->getSize();

  Card* card = cards_[size - indexFromBack];

  if (faceUp) {
    if (!card->isFaceUp()) {
      card->flip();  // Flip to face-up
      return true;   // Successfully flipped
    }
  } else {
    if (card->isFaceUp()) {
      card->flip();  // Flip to face-down
    }
  }

  return false;  // No action taken
}

int Pile::cardIndexFromTop(Card* card) const {
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

// GUI RELATED FUNCTIONS

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
