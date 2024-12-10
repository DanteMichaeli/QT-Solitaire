#ifndef TARGETPILE_HPP
#define TARGETPILE_HPP

#include "pile.hpp"

using namespace std;

/**
 * @class TargetPile
 * @brief Represents a target pile in solitaire, where cards of a specific suit
 * are stacked in ascending order.
 *
 * In solitaire games, the goal for target piles is to build a sequence of cards
 * from ACE up to KING, with all cards belonging to the same suit.
 */
class TargetPile : public Pile {
  Q_OBJECT
 public:
  /**
   * @brief Constructs a TargetPile for a specific suit.
   * @param suit The suit that this TargetPile will accept (CLUBS, DIAMONDS,
   * HEARTS, or SPADES).
   */
  TargetPile(QGraphicsItem *parent = nullptr);

  /**
   * @brief Checks if a card can be legally added to this pile.
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
  bool IsValid(const Card &card) override;

  void updateVisuals() override;

  void setCardPrevScenePos() override;

 protected:
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget = nullptr) override;

 private:
  Suit suit_;  ///< The suit required for cards in this TargetPile (CLUBS,
               ///< DIAMONDS, HEARTS, or SPADES).
};

#endif
