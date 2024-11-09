#ifndef KLONDIKEPILE_HPP
#define KLONDIKEPILE_HPP

#include "pile.hpp"

/**
 * @class KlondikePile
 * @brief A specialized type of Pile used in the game of Klondike Solitaire.
 *        This pile has specific rules for adding and removing cards, enforcing
 * the Klondike game rules.
 */
class KlondikePile : public Pile {
 public:
  /**
   * @brief Default constructor for an empty pile.
   */
  KlondikePile() {}

  /**
   * @brief Constructs a pile with a specified number of cards dealt from a
   * deck. A certain number of cards are set to face up.
   * @param deck The deck from which cards are dealt.
   * @param nofCards The total number of cards to add to the pile.
   * @param nofFacingUp The number of cards to flip face-up, starting from the
   * top.
   */
  KlondikePile(Deck& deck, size_t nofCards, size_t nofFacingUp)
      : Pile(deck, nofCards, nofFacingUp) {}

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
  bool IsValid(const Card& card) override {
    if (!card.isFaceUp()) {
      return false;
    }
    if (cards_.empty()) {
      return card.GetRank() == Rank::KING;
    }
    Card top = *cards_.back();
    bool diffColor = card.GetColor() != top.GetColor();
    bool isNextLower = card.GetRank() == top.GetRank() - 1;
    return diffColor && isNextLower;
  }

  /**
   * @brief Attempt to move a specified number of cards to
   * another KlondikePile.
   * @param other The destination to which cards are moved to.
   * @param nofCards The number of cards to move.
   * @return true if the cards are successfully moved, false otherwise.
   */
  bool TransferSubPile(KlondikePile& other, size_t nofCards) {
    if (nofCards > cards_.size()) {
      return false;
    }
    size_t start = cards_.size() - nofCards;
    if (!other.IsValid(*cards_[start])) {
      return false;
    }
    for (size_t i = start; i < cards_.size(); i++) {
      other.AddCard(move(cards_[i]));
    }
    for (size_t j = 0; j < nofCards; j++) {
      RemoveCard();
    }
    return true;
  }

  /**
   * @brief Removes the top card from the pile and returns it.
   *        If the new top card is face-down, it flips it face-up.
   * @return A unique pointer to the removed Card, or nullptr if the pile is
   * empty.
   */
  unique_ptr<Card> RemoveCard() override {
    auto cardPtr = Pile::RemoveCard();
    if (!cards_.empty()) {
      cards_.back()->flipUp();
    }
    return cardPtr;
  }
};

#endif
