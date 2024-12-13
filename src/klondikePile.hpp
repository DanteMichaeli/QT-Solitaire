#ifndef KLONDIKEPILE_HPP
#define KLONDIKEPILE_HPP

#include "pile.hpp"

/**
 * @class KlondikePile
 * @brief A special type of Pile in the game of Klondike Solitaire.
 *        This pile enforces the rules of Klondike Solitaire for handling cards.
 */
class KlondikePile : public Pile {
  Q_OBJECT

 public:
  /**
   * @defgroup KlondikePileLogic Klondike Pile Logic
   * @brief Members related to the Klondike pile's logical state.
   * @{
   */

  /**
   * @brief Construct a pile with a specified number of cards dealt from a
   * deck, a subset of which are dealt face up.
   * @param deck The deck from which cards are dealt.
   * @param nofCards The total number of cards to add to the pile.
   * @param nofFacingUp The number of cards to flip face-up, starting from the
   * top.
   */
  KlondikePile(QGraphicsItem* parent = nullptr);

  /**
   * @brief Check if a card can be legally added to this pile based on Klondike
   * Solitaire rules.
   * @param card The card to check for validity.
   * @return true if the card can be added, false otherwise.
   *
   * A card can be added to the pile if:
   * - The pile is empty and the card is a KING.
   * - The pile is non-empty, and the card is of opposite color and one rank
   * lower than the top card.
   */
  bool isValid(const Card& card) override;

  /** @} */  // End of KlondikePileLogic

  /**
   * @defgroup KlondikePileGUI Klondike Pile GUI
   * @brief Members related to the Klondike pile's graphical representation.
   * @{
   */

  /**
   * @brief Update the visual representation of the pile.
   */
  void updateVisuals() override;

  /**
   * @brief Set the previous scene positions of all cards in the pile (in scene
   * coordinates). For a KlondikePile, this is the pile's position plus an
   * offset.
   */
  void setCardsPrevScenePos() override;

 private:
  /**
   * @brief Return the bounding rectangle of the item. Defines the area within
   * which the item can be drawn and interacted with.
   *
   * @return A QRectF object specifying the bounding rectangle.
   */
  QRectF boundingRect() const override;

  /**
   * @brief Get the offset used for positioning cards in the pile.
   * @return The offset as a QPointF.
   */
  QPointF getOffset() const override;

  /** @} */  // End of KlondikePileGUI
};

#endif
