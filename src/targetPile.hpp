#ifndef TARGETPILE_HPP
#define TARGETPILE_HPP

#include "pile.hpp"

using namespace std;

/**
 * @class TargetPile
 * @brief A special type of Pile in solitaire, where cards of a specific suit
 * are stacked in ascending order.
 *
 * In solitaire games, the goal for target piles is to build a sequence of cards
 * from ACE up to KING, with all cards belonging to the same suit.
 */
class TargetPile : public Pile {
  Q_OBJECT
 public:
  /**
   * @defgroup TargetPileLogic Target Pile Logic
   * @brief Members related to the Target pile's logical state.
   * @{
   */

  /**
   * @brief Construct a TargetPile for a specific suit.
   * @param suit The suit that this TargetPile will accept (CLUBS, DIAMONDS,
   * HEARTS, or SPADES).
   */
  TargetPile(QGraphicsItem *parent = nullptr);

  /**
   * @brief Check if a card can be legally added to this pile.
   * @param card The card to check for validity.
   * @return true if the card matches the suit of the pile and continues the
   * ascending sequence, false otherwise.
   *
   * A card is valid if:
   * - It is face-up.
   * - It matches the suit of the TargetPile.
   * - If the pile is empty, the card must be an ACE.
   * - If the pile is non-empty, the card must have a rank exactly one higher
   * than the current top card.
   */

  bool isValid(const Card &card) override;

  /** @} */  // End of TargetPileLogic

  /**
   * @defgroup TargetPileGUI Target Pile GUI
   * @brief Members related to the Target pile's graphical representation.
   * @{
   */

  /**
   * @brief Update the visual representation of the pile.
   */
  void updateVisuals() override;

  /**
   * @brief Set the previous scene positions of all cards in the pile (in scene
   * coordinates). For a TargetPile there is no offset.
   */
  void setCardsPrevScenePos() override;

  /** @} */  // End of TargetPileGUI

 private:
  /**
   * @addtogroup TargetPileLogic
   * @{
   */

  Suit suit_;  ///< The suit required for cards in this TargetPile (CLUBS,
               ///< DIAMONDS, HEARTS, or SPADES).

  /** @} */  // End of TargetPileLogic

  /**
   * @addtogroup TargetPileGUI
   * @{
   */

  /**
   * @brief Paint the pile slot on the scene.
   * @param painter Pointer to the QPainter used for drawing.
   * @param option Provides style options for the item.
   * @param widget Optional pointer to the widget being painted on.
   */
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget = nullptr) override;

  /**
   * @brief Get the offset used for positioning cards in the pile.
   * @return The offset as a QPointF.
   */
  QPointF getOffset() const override;

  /** @} */  // End of TargetPileGUI
};

#endif
