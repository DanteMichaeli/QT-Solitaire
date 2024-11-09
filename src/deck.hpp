#ifndef DECK_HPP
#define DECK_HPP

#include <algorithm>
#include <chrono>
#include <exception>
#include <memory>
#include <random>
#include <vector>

#include "card.hpp"
using namespace std;

/**
 * @brief Array of all possible suits, used to initialize a deck.
 */
const Suit allSuits[] = {CLUBS, DIAMONDS, SPADES, HEARTS};

/**
 * @brief Array of all possible ranks, used to initialize a deck.
 */
const Rank allRanks[] = {ACE,   TWO,  THREE, FOUR, FIVE,  SIX, SEVEN,
                         EIGHT, NINE, TEN,   JACK, QUEEN, KING};

/**
 * @class Deck
 * @brief Represents a deck of cards with operations for shuffling, dealing, and
 * checking status.
 */
class Deck {
 public:
  /**
   * @brief Constructs a standard deck of 52 cards, each unique by suit and
   * rank.
   */
  Deck() {
    for (Suit suit : allSuits) {
      for (Rank rank : allRanks) {
        cards_.push_back(make_unique<Card>(Card(suit, rank)));
      }
    }
  }

  /**
   * @brief Destroys the Deck object and outputs a message indicating the deck
   * is being destroyed.
   */
  ~Deck() { cout << "Deck destroyed" << endl; }

  /**
   * @brief Shuffles the deck of cards using a random seed.
   * @param seed Optional seed for reproducible shuffling. If 0 (default), the
   * seed is based on system time.
   */
  void Shuffle(unsigned long seed = 0) {
    if (seed == 0) {
      // Default seed is based on system time.
      seed = chrono::high_resolution_clock::now().time_since_epoch().count();
    }
    mt19937 generator(seed);
    shuffle(cards_.begin(), cards_.end(), generator);
  }

  /**
   * @brief Peeks at the top card of the deck without removing it.
   * @return A reference to the top Card object.
   * @throws std::out_of_range if the deck is empty.
   */
  Card& Peek() const {
    if (cards_.empty()) {
      throw std::out_of_range("Deck is empty");
    }
    return *cards_.back();
  }

  /**
   * @brief Deals (removes) the top card from the deck.
   * @return A unique pointer to the top Card object. Returns nullptr if the
   * deck is empty.
   */
  unique_ptr<Card> dealCard() {
    if (cards_.empty()) {
      return nullptr;
    }
    unique_ptr<Card> cardPtr = move(cards_.back());
    cards_.pop_back();
    return cardPtr;
  }

  /**
   * @brief Checks if the deck is empty.
   * @return true if the deck contains no cards, false otherwise.
   */
  bool Empty() const { return cards_.empty(); }

  /**
   * @brief Adds a face down card to the top of the deck.
   * @param card A unique pointer to the Card object to be added.
   */
  void AddCard(unique_ptr<Card> card) {
    card->flipDown();
    cards_.push_back(move(card));
  }

 private:
  /**
   * @brief Collection of unique pointers to Card objects, representing the
   * cards in the deck. A vector is used for easy shuffling.
   */
  vector<unique_ptr<Card>> cards_;
};

#endif
