#ifndef WASTEPILE_HPP
#define WASTEPILE_HPP

#include "pile.hpp"

using namespace std;

class Deck;

/**
 * @class WastePile
 * @brief A special type of Pile in a solitaire game, where discarded or
 * drawn cards are temporarily held.
 *
 * The waste pile has specific behavior in solitaire games:
 * - Cards from the deck are added face-up.
 * - Cards can be transferred back to the deck, optionally with shuffling.
 */
class WastePile : public Pile {
  Q_OBJECT
 public:
  /**
   * @defgroup WastePileLogic Waste Pile Logic
   * @brief Members related to the Waste pile's logical state.
   * @{
   */

  /**
   * @brief Construct an empty Waste pile.
   */
  WastePile(QGraphicsItem* parent = nullptr);

  /**
   * @brief Check if a card can be legally added to this pile based on game
   * rules. In WastePile, cards cannot be added directly by any method other
   * than `AddFromDeck`.
   * @param card The card to check for validity.
   * @return Always returns false for WastePile.
   */
  bool isValid(const Card& card) override;

  /**
   * @brief Add a specified number of cards from the deck to the waste pile,
   * flipping them face-up.
   * @param deck The deck from which cards are dealt.
   * @param nofCards The number of cards to add to the waste pile.
   */
  int addFromDeck(Deck& deck, const unsigned int nofCards);

  /**
   * @brief Transfer all cards from the waste pile back to the deck.
   * @param deck The deck to transfer the cards to.
   * @param shuffle Whether to shuffle the cards in the deck after transfer.
   */
  void undoAddFromDeck(Deck& deck, const unsigned int nofCards);

  /** @} */  // End of WastePileLogic

  /**
   * @defgroup WastePileGUI Waste Pile GUI
   * @brief Members related to the Waste pile's graphical representation.
   * @{
   */

  /**
   * @brief Update the visual representation of the pile.
   */
  void updateVisuals() override;

  /**
   * @brief Set the previous scene positions of all cards in the pile (in scene
   * coordinates). For a WastePile, this is the pile's position plus an offset.
   */
  void setCardsPrevScenePos() override;

 private:
  /**
   * @brief Get the offset for the waste pile.
   * @return The offset for the waste pile.
   */
  QPointF getOffset() const override;

  /** @} */  // End of WastePileGUI
};

#endif
