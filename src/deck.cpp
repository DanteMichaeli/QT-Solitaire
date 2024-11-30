#include "deck.hpp"

#include <QDebug>

#include "wastePile.hpp"

Deck::Deck(QGraphicsItem* parent) : Pile(parent) {
  // Add cards to deck
  for (Suit suit : allSuits) {
    for (Rank rank : allRanks) {
      qDebug() << "add" << rank;
      auto card = std::make_unique<Card>(suit, rank);
      // card->hide();
      AddCard(card);
    }
  }
  // getTopCard().show();
  qDebug() << "Size:" << Size();
  qDebug() << "Cards:" << cards_.size();
  updateVisuals();
}

void Deck::Shuffle(unsigned long seed) {
  if (seed == 0) {
    // Default seed is based on system time.
    seed = chrono::high_resolution_clock::now().time_since_epoch().count();
  }
  mt19937 generator(seed);
  shuffle(cards_.begin(), cards_.end(), generator);
}

// TODO:
bool Deck::IsValid(const Card& card) {
  if (static_cast<WastePile*>(card.parentItem()) != nullptr) {
    return true;
  }
  return false;
}

void Deck::updateVisuals() {}

void Deck::mousePressEvent(QGraphicsSceneMouseEvent* event) {
  emit deckClicked();
}

void Deck::onCardClicked(Card* card) { emit deckClicked(); }