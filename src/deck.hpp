#ifndef DECK_HPP
#define DECK_HPP

#include "pile.hpp"

using namespace std;

class WastePile;

/**
 * @class Deck
 * @brief Represents a deck of cards with operations for shuffling, dealing, and
 * status checking.
 */
class Deck : public Pile {
  Q_OBJECT
 public:
  /**
   * @defgroup DeckLogic Deck Logic
   * @brief Members related to the deck's logical state.
   * @{
   */

  /**
   * @brief Construct a standard deck of 52 cards, each unique by suit and
   * rank.
   */
  explicit Deck(QGraphicsItem* parent = nullptr);

  /**
   * @brief Add all cards from waste pile to the deck.
   * @param pile Waste pile that the cards are gathered from.
   * @return true if waste pile is not empty.
   * @return false otherwise.
   */
  bool recycle(WastePile& pile);

  /**
   * @brief Undo the recycle operation
   */
  void undoRecycle(WastePile& pile);

  /**
   * @brief Shuffle the deck using a random seed.
   * @param seed Optional seed for reproducible shuffling. If 0 (default), the
   * seed is based on system time.
   */
  template <typename T>
  static void shuffle(vector<T>& arr, unsigned long seed = 0);

  /**
   * @brief Check if card can be legally added to deck. Override Pile::isValid.
   *
   * @param card
   * @return false always, as cards cannot be added directly to the deck.
   */
  bool isValid(const Card& card) override;

  /** @} */  // End of DeckLogic

  /**
   * @defgroup DeckGUI Deck GUI
   * @brief Members related to the deck's graphical representation.
   * @{
   */

  /**
   * @brief Set the previous position of all cards in the deck to the deck's
   * position (in scene coordinates).
   */
  void setCardsPrevScenePos() override;

  /**
   * @brief Update the visual representation of the deck.
   */
  void updateVisuals() override;

  /**
   * @brief Get the offset for positioning cards in the deck (none).
   * @return The offset as a QPointF
   */
  QPointF getOffset() const override;

  /** @} */  // End of DeckGUI

 private:
  /**
   * @addtogroup DeckGUI
   * @{
   */

  /**
   * @brief Handle mouse press events on the deck.
   * @param event The mouse event triggered by clicking on the deck.
   */
  void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

  /** @} */  // End of DeckGUI
};

#endif
