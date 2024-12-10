#ifndef CARD_HPP
#define CARD_HPP

#include <QGraphicsDropShadowEffect>
#include <QGraphicsObject>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QPropertyAnimation>
#include <QTimer>
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
  Q_PROPERTY(qreal glowRadius READ glowRadius WRITE setGlowRadius)
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

  const QPixmap& getPixmap() const { return pixmap_; }

  /**
   * @brief Returns a Qstring representation of the card, including its rank and
   * suit.
   * @return String in the format "RANK_of_SUIT".
   */
  QString cardToQString() const;

  /**
   * @brief Sets the card to face-up. Also repaints the pixmap.
   */
  void flipUp();

  /**
   * @brief Sets the card to face-down. Also repaints the pixmap.
   */
  void flipDown();

  /**
   * @brief Toggle card face. Also repaints the pixmap.
   */
  void toggleFace();

  /**
   * @brief Check whether this card is eligible to recieve mouse clicks.
   * @return true
   * @return false
   */
  bool isClickable();

  /**
   * @brief Check whether this card is movable (/draggable).
   * @return true
   * @return false
   */
  bool isDraggable();

  bool operator==(Card& card);

  qreal glowRadius() const { return glowEffect_->blurRadius(); }

  void setGlowRadius(qreal radius) { glowEffect_->setBlurRadius(radius); }

  void startGlowing();

  void animateMove(QPointF& startPos, QPointF& endPos);

  void setPrevScenePos(QPointF& pos) { prevScenePos_ = pos; }

  const QPointF& getPrevScenePos() const { return prevScenePos_; }

 signals:
  /**
   * @brief Signal emitted when a card is clicked.
   * @param card Pointer to the card that was clicked.
   */
  void cardClicked(Card* card);

  /**
   * @brief Signal emitted when a card is dragged to a new position.
   * @param card Pointer to the card being dragged.
   * @param newScenePos The new position of the card in scene coordinates.
   */
  void cardDragged(Card* card, const QPointF& newScenePos);

 protected:
  /**
   * @brief Returns the bounding rectangle of the card item.
   *
   * This function defines the bounding area for the card, which is used
   * for collision detection.
   *
   * @return A QRectF object defining the bounding rectangle.
   */
  QRectF boundingRect() const override;

  /**
   * @brief Paints the card item on the scene.
   *
   * This function is called during the rendering process to draw the
   * card's appearance using the provided painter.
   *
   * @param painter Pointer to the QPainter used for drawing.
   * @param option Provides style options for the item.
   * @param widget Optional pointer to the widget being painted on.
   */
  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
             QWidget* widget = nullptr) override;

  /**
   * @brief Handles mouse press events on the card.
   * @param event Pointer to the mouse event object.
   */
  void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

  /**
   * @brief Handles mouse move events on the card.
   * @param event Pointer to the mouse event object.
   */
  void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

  /**
   * @brief Handles mouse release events on the card.
   * @param event Pointer to the mouse event object.
   */
  void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

  void startGlowingOut();

  void stopGlowingAnimation();

 private:
  void createDragPixmap();

  QPixmap frontImage_;  ///< The front image of the card.
  QPixmap backImage_;   ///< The back image of the card.
  QPixmap pixmap_;      ///< Current pixmap.
  QPointF prevPos_;     ///< The previous position of the card.

  QGraphicsDropShadowEffect* glowEffect_;
  QPropertyAnimation* glowIn_;
  QPropertyAnimation* glowOut_;
  QTimer* glowTimer_;
  bool isGlowing_;

  Suit suit_;    ///< Suit of the card (CLUBS, DIAMONDS, SPADES, HEARTS).
  Rank rank_;    ///< Rank of the card (ACE to KING).
  bool faceUp_;  ///< Face-up status of the card.
  Color color_;  ///< Color of the card (BLACK or RED).
  vector<Card*> cardsAbove_;  ///< vector of all the cards that are on top of
                              ///< this card in a pile.
  QPropertyAnimation* animation_;
  QPointF prevScenePos_;

  QPixmap tmpDragMap_;  // currently not in use.
};

#endif
