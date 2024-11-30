#ifndef CARD_HPP
#define CARD_HPP

#include <QGraphicsObject>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <iostream>
#include <sstream>
#include <string>

#define SCALING_FACTOR 0.2

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
 * @brief Array of all possible suits, used to initialize a deck.
 */
constexpr Suit allSuits[] = {CLUBS, DIAMONDS, SPADES, HEARTS};

/**
 * @brief Array of all possible ranks, used to initialize a deck.
 */
constexpr Rank allRanks[] = {ACE,   TWO,  THREE, FOUR, FIVE,  SIX, SEVEN,
                             EIGHT, NINE, TEN,   JACK, QUEEN, KING};

/**
 * @class Card
 * @brief Represents a single playing card with a suit, rank, color, and face-up
 * status.
 */
class Card : public QGraphicsObject {
  Q_OBJECT
 public:
  /**
   * @brief Constructs a new Card object with specified suit and rank.
   * @param s Suit of the card (CLUBS, DIAMONDS, SPADES, HEARTS).
   * @param r Rank of the card (ACE to KING).
   */
  explicit Card(Suit s, Rank r, QGraphicsItem* parent = nullptr);

  /**
   * @brief Destroys the Card object and outputs a message indicating which card
   * is being destroyed.
   */
  ~Card();

  /**
   * @brief Updates the image shown based on the value of faceUp_.
   */
  void updateCardImage();

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
   * @brief Get the Suit in QString
   * @return QString
   */
  QString getSuitQstring() const;

  /**
   * @brief Get the Rank in QString
   * @return QString
   */
  QString getRankQstring() const;

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
   * @brief Returns a Qstring representation of the card, including its rank and
   * suit.
   * @return String in the format "RANK_of_SUIT".
   */
  QString cardToQString() const;

  /**
   * @brief Sets the card to face-up.
   */
  void flipUp();

  /**
   * @brief Sets the card to face-down.
   */
  void flipDown();

  bool isClickable();
  bool isDraggable();

 signals:
  void cardClicked(Card* card);
  void cardDragged(Card* card, const QPointF& newPosition);

 protected:
  QRectF boundingRect() const override;

  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
             QWidget* widget = nullptr) override;

  void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

 private:
  QPixmap frontImage_;  ///< The front image of the card.
  QPixmap backImage_;   ///< The back image of the card.
  QPixmap pixmap_;      ///< Current pixmap.

  QPointF prevPos_;
  vector<Card*> selectedSubpile_;

  Suit suit_;    ///< Suit of the card (CLUBS, DIAMONDS, SPADES, HEARTS).
  Rank rank_;    ///< Rank of the card (ACE to KING).
  bool faceUp_;  ///< Face-up status of the card.
  Color color_;  ///< Color of the card (BLACK or RED).
};

#endif
