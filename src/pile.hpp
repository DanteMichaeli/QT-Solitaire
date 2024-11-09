#ifndef PILE_HPP
#define PILE_HPP

#include "card.hpp"
#include "deck.hpp"

using namespace std;

/**
 * @class Pile
 * @brief Represents a pile of cards with the ability to add, remove, and
 * transfer cards. This is an abstract base class, as it includes a pure virtual
 * method IsValid.
 */
class Pile {
 public:
  /**
   * @brief Default constructor for an empty pile.
   */
  Pile() {}

  /**
   * @brief Constructs a pile with a specified number of cards dealt from a
   * deck. A certain number of cards are set to face up.
   * @param deck The deck from which cards are dealt.
   * @param nofCards The total number of cards to add to the pile.
   * @param nofFacingUp The number of cards to flip face-up, starting from the
   * top.
   */
  Pile(Deck& deck, size_t nofCards, size_t nofFacingUp) {
    for (size_t i = 0; i < nofCards; i++) {
      unique_ptr<Card> card = deck.dealCard();
      cards_.push_back(card);
      if (i >= nofFacingUp) {
        card->flipUp();
      }
    }
  }

  /**
   * @brief Destructor for the Pile, outputs a message when the pile is
   * destroyed.
   */
  virtual ~Pile() { cout << "Pile destroyed" << endl; }

  /**
   * @brief Add a card to the pile.
   * @param card A unique pointer to the Card to add.
   */
  void AddCard(std::unique_ptr<Card> card) { cards_.push_back(move(card)); }

  /**
   * @brief A function that moves a card to another pile.
   * @param other The target pile to which cards are moved to.
   * @return true if the card is successfully moved, false otherwise.
   */
  virtual bool TransferCard(Pile& other) {
    if (!cards_.empty()) {
      return false;
    }
    if (!other.IsValid(*cards_.back())) {
      return false;
    }
    other.AddCard(move(cards_.back()));
    RemoveCard();
    return true;
  }

  /**
   * @brief Removes and returns the top card from the pile.
   * @return A unique pointer to the top Card object, or nullptr if the pile
   * is empty.
   */
  virtual unique_ptr<Card> RemoveCard() {
    if (cards_.empty()) {
      return nullptr;
    }
    unique_ptr<Card> cardPtr = move(cards_.back());
    cards_.pop_back();
    return cardPtr;
  }

  /**
   * @brief Pure virtual function to check if a card can be added to the pile.
   *        Must be implemented by derived classes.
   * @param card The card to check.
   * @return true if the card can be added, false otherwise.
   */
  virtual bool IsValid(const Card& card) = 0;

 protected:
  /**
   * @brief Collection of unique pointers to Card objects in the pile.
   */
  vector<unique_ptr<Card>> cards_;
};

#endif
