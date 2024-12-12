#ifndef DECK_HPP
#define DECK_HPP

#include <algorithm>
#include <chrono>
#include <exception>
#include <memory>
#include <random>
#include <vector>

#include "pile.hpp"

using namespace std;

class WastePile;

/**
 * @class Deck
 * @brief Represents a deck of cards with operations for shuffling, dealing, and
 * checking status.
 */
class Deck : public Pile {
  Q_OBJECT
 public:
  /**
   * @brief Constructs a standard deck of 52 cards, each unique by suit and
   * rank.
   */
  explicit Deck(QGraphicsItem* parent = nullptr);

  /**
   * @brief Shuffles the deck of cards using a random seed.
   * @param seed Optional seed for reproducible shuffling. If 0 (default), the
   * seed is based on system time.
   */
  template <typename T>
  static void Shuffle(vector<T>& arr, unsigned long seed = 0);

  /**
   * @brief Add all cards from waste pile to the deck.
   * @param pile Waste pile that the cards are gathered from.
   * @return true if waste pile is not empty.
   * @return false otherwise.
   */
  bool recycle(WastePile& pile);

  void undoRecycle(WastePile& pile);

  void setCardPrevScenePos() override;

  bool IsValid(const Card& card) override;

  void updateVisuals() override;

  QPointF getOffset() const override;

 protected:
  /**
   * @brief Handles mouse press events on the deck.
   * @param event The mouse event triggered by clicking on the deck.
   */
  void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
};

#endif
