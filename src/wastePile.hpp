#ifndef WASTEPILE_HPP
#define WASTEPILE_HPP

#include "pile.hpp"

using namespace std;

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
 public:
  /**
   * @brief Default constructor for an empty pile.
   */
  WastePile() {}

  /**
   * @brief Checks if a card can be legally added to this pile based on game
   * rules. In WastePile, cards cannot be added directly by any method other
   * than `AddFromDeck`.
   * @param card The card to check for validity.
   * @return Always returns false for WastePile.
   */
  bool IsValid(const Card& card) override { return false; }

  /**
   * @brief Adds a specified number of cards from the deck to the waste pile,
   * flipping them face-up.
   * @param deck The deck from which cards are dealt.
   * @param nofCards The number of cards to add to the waste pile.
   */
  void AddFromDeck(Deck& deck, size_t nofCards) {
    for (size_t i = 0; i < nofCards; i++) {
      auto cardPtr = deck.dealCard();
      if (cardPtr == nullptr) {
        break;
      }
      cardPtr->flipUp();
      cards_.push_back(cardPtr);
    }
  }

  /**
   * @brief Moves all cards from the waste pile back to the deck.
   * @param deck The deck to which the waste pile cards are added.
   * @param shuffle Default value is false but if set to true, shuffles the deck
   * after transferring the cards.
   * @return true if the transfer is successful, false if the deck is not empty.
   */
  bool AddToDeck(Deck& deck, bool shuffle = false) {
    if (deck.Empty()) {
      while (!cards_.empty()) {
        deck.AddCard(move(cards_.back()));
        cards_.pop_back();
      }
      if (shuffle) {
        deck.Shuffle();
      }
      return true;
    }
    return false;
  }
};

#endif
