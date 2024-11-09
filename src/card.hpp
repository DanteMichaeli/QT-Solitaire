#ifndef CARD_HPP
#define CARD_HPP

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

/**
 * @brief Enumeration for the four suits in a deck of cards.
 */
enum Suit { CLUBS, DIAMONDS, SPADES, HEARTS };

/**
 * @brief Enumeration for the ranks in a deck of cards, with ACE set to 1 and
 * KING as the highest rank.
 */
enum Rank {
  ACE = 1,
  TWO,
  THREE,
  FOUR,
  FIVE,
  SIX,
  SEVEN,
  EIGHT,
  NINE,
  TEN,
  JACK,
  QUEEN,
  KING
};

/**
 * @brief Enumeration for the color of a card, either BLACK or RED.
 */
enum Color { BLACK, RED };

/**
 * @class Card
 * @brief Represents a single playing card with a suit, rank, color, and face-up
 * status.
 */
class Card {
 public:
  /**
   * @brief Constructs a new Card object with specified suit and rank.
   * @param s Suit of the card (CLUBS, DIAMONDS, SPADES, HEARTS).
   * @param r Rank of the card (ACE to KING).
   */
  Card(Suit s, Rank r) : suit_(s), rank_(r), faceUp_(false) {
    switch (s) {
      case Suit::CLUBS:
        color_ = Color::BLACK;
        break;
      case Suit::DIAMONDS:
        color_ = Color::RED;
        break;
      case Suit::HEARTS:
        color_ = Color::RED;
        break;
      case Suit::SPADES:
        color_ = Color::BLACK;
        break;
    }
  }

  /**
   * @brief Destroys the Card object and outputs a message indicating which card
   * is being destroyed.
   */
  ~Card() { cout << "Card destroyed: " << CardToString() << endl; }

  /**
   * @brief Gets the suit of the card.
   * @return Suit of the card.
   */
  Suit GetSuit() const { return suit_; }

  /**
   * @brief Gets the rank of the card.
   * @return Rank of the card.
   */
  Rank GetRank() const { return rank_; }

  /**
   * @brief Checks if the card is face-up.
   * @return true if the card is face-up, false otherwise.
   */
  bool isFaceUp() const { return faceUp_; }

  /**
   * @brief Gets the color of the card.
   * @return Color of the card (BLACK or RED).
   */
  Color GetColor() const { return color_; }

  /**
   * @brief Returns a string representation of the card, including its rank and
   * suit.
   * @return String in the format "RANK of SUIT".
   */
  string CardToString() const {
    stringstream ss;
    switch (rank_) {
      case Rank::ACE:
        ss << "ACE";
        break;
      case Rank::JACK:
        ss << "JACK";
        break;
      case Rank::QUEEN:
        ss << "QUEEN";
        break;
      case Rank::KING:
        ss << "KING";
        break;
      default:
        ss << rank_;
    }
    ss << " of ";

    switch (suit_) {
      case Suit::CLUBS:
        ss << "CLUBS";
        break;
      case Suit::DIAMONDS:
        ss << "DIAMONDS";
        break;
      case Suit::HEARTS:
        ss << "HEARTS";
        break;
      case Suit::SPADES:
        ss << "SPADES";
        break;
    }
    return ss.str();
  }

  /**
   * @brief Flips the face-up status of the card (face-up becomes face-down and
   * vice versa).
   */
  void flipFace() { faceUp_ ^= true; }

  /**
   * @brief Sets the card to face-up.
   */
  void flipUp() { faceUp_ = true; }

  /**
   * @brief Sets the card to face-down.
   */
  void flipDown() { faceUp_ = false; }

 private:
  Suit suit_;    ///< Suit of the card (CLUBS, DIAMONDS, SPADES, HEARTS).
  Rank rank_;    ///< Rank of the card (ACE to KING).
  bool faceUp_;  ///< Face-up status of the card.
  Color color_;  ///< Color of the card (BLACK or RED).
};

#endif
