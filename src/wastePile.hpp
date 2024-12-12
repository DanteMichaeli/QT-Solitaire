#ifndef WASTEPILE_HPP
#define WASTEPILE_HPP

#include "pile.hpp"

using namespace std;

class Deck;

/**
 * @class WastePile
 * @brief Represents the waste pile in a solitaire game, where discarded or
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
   * @brief Default constructor for an empty pile.
   */
  WastePile(QGraphicsItem* parent = nullptr);

  /**
   * @brief Checks if a card can be legally added to this pile based on game
   * rules. In WastePile, cards cannot be added directly by any method other
   * than `AddFromDeck`.
   * @param card The card to check for validity.
   * @return Always returns false for WastePile.
   */
  bool IsValid(const Card& card) override;

  /**
   * @brief Adds a specified number of cards from the deck to the waste pile,
   * flipping them face-up.
   * @param deck The deck from which cards are dealt.
   * @param nofCards The number of cards to add to the waste pile.
   */
  int AddFromDeck(Deck& deck, size_t nofCards);

  void undoAddFromDeck(Deck& deck, size_t nofCards);

  void updateVisuals() override;

  void setCardPrevScenePos() override;

  QPointF getOffset() const override;
};

#endif
