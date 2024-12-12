#ifndef KLONDIKEPILE_HPP
#define KLONDIKEPILE_HPP

#include "deck.hpp"
#include "pile.hpp"

/**
 * @class KlondikePile
 * @brief A specialized type of Pile used in the game of Klondike Solitaire.
 *        This pile has specific rules for adding and removing cards, enforcing
 * the Klondike game rules.
 */
class KlondikePile : public Pile {
  Q_OBJECT
 public:
  /**
   * @brief Constructs a pile with a specified number of cards dealt from a
   * deck. A certain number of cards are set to face up.
   * @param deck The deck from which cards are dealt.
   * @param nofCards The total number of cards to add to the pile.
   * @param nofFacingUp The number of cards to flip face-up, starting from the
   * top.
   */
  KlondikePile(QGraphicsItem* parent = nullptr);

  /**
   * @brief Flips the top card up/down.
   *
   * In game class, if flipping is succesfull, adds points for the player.
   *
   * @param value Boolean value. True for flip up, false for down.
   * @return true when card is flipped up.
   * @return false otherwise.
   */
  bool flipCard(bool faceUp, int indexFromBack = 1);

  /**
   * @brief Checks if a card can be legally added to this pile based on Klondike
   * Solitaire rules.
   * @param card The card to check for validity.
   * @return true if the card can be added, false otherwise.
   *
   * A card can be added to the pile if:
   * - The pile is empty and the card is a KING.
   * - The pile is non-empty, and the card is of opposite color and one rank
   * lower than the top card.
   */
  bool IsValid(const Card& card) override;

  void updateVisuals() override;

  void setCardPrevScenePos() override;

  QPointF getOffset() const override;

 protected:
  QRectF boundingRect() const override;
};

#endif
