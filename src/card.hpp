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
  Q_PROPERTY(qreal getGlowRadius READ getGlowRadius WRITE setGlowRadius)
  Q_PROPERTY(qreal getFlipProgress READ getFlipProgress WRITE setFlipProgress)

 public:
  /**
   * @addtogroup CardLogic
   * @{
   */

  /**
   * @brief Construct a new Card object with specified suit and rank.
   * @param s Suit of the card (CLUBS, DIAMONDS, SPADES, HEARTS).
   * @param r Rank of the card (ACE to KING).
   */
  explicit Card(Suit s, Rank r, QGraphicsItem* parent = nullptr);

  /**
   * @brief Destroy the Card object
   */
  ~Card();

  /**
   * @brief Get the suit of the card.
   * @return Suit of the card.
   */
  Suit getSuit() const { return suit_; }

  /**
   * @brief Get the rank of the card.
   * @return Rank of the card.
   */
  Rank getRank() const { return rank_; }

  /**
   * @brief Get the Suit in QString
   * @return QString of the suit
   */
  QString getSuitQstring() const;

  /**
   * @brief Get the Rank in QString
   * @return QString of the rank
   */
  QString getRankQstring() const;

  /**
   * @brief Checks if the card is face-up.
   * @return true if the card is face-up, false otherwise.
   */
  bool isFaceUp() const { return faceUp_; }

  /**
   * @brief Get the color of the card.
   * @return Color of the card (BLACK or RED).
   */
  Color getColor() const { return color_; }

  /**
   * @brief Return a Qstring representation of the card, including its rank and
   * suit.
   * @return String in the format "RANK_of_SUIT".
   */
  QString cardToQString() const;

  /**
   * @brief Flip the card up/down and repaint the pixmap.
   */
  void flip();

  /**
   * @brief Check whether this card is eligible to recieve mouse clicks.
   * @return true if the card is clickable, false otherwise.
   */
  bool isClickable();

  /**
   * @brief Check whether this card is draggable.
   * @return true if the card is draggable, false otherwise.
   */
  bool isDraggable();

  /**
   * @brief Set the previous position of the card.
   * @param pos QPointF
   */
  void setPrevScenePos(QPointF& pos) { prevScenePos_ = pos; }

  /**
   * @brief Get the previous position of the card.
   * @return QPointF of the previous position.
   */
  const QPointF& getPrevScenePos() const { return prevScenePos_; }

  /** @} */  // End of CardLogic

  /**
   * @addtogroup CardGUI
   * @{
   */

  /**
   * @brief Start the moving animation.
   */
  void animateMove(QPointF& startPos, QPointF& endPos);

  /**
   * @brief Start the glowing animation.
   */
  void animateGlowIn();

  /**
   * @brief Stop the glowing animation.
   */
  void animateGlowOut();

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

  /** @} */  // End of CardGUI

 private:
  /**
   * @defgroup CardLogic Card Logic
   * @brief Variables related to the card's logical state.
   * @{
   */

  QPointF prevPos_;  ///< The previous position of the card in the scene.
  vector<Card*> cardsAbove_;  ///< Cards that are above this card in a pile.
  Suit suit_;    ///< Suit of the card (CLUBS, DIAMONDS, SPADES, HEARTS).
  Rank rank_;    ///< Rank of the card (ACE to KING).
  bool faceUp_;  ///< Face-up status of the card.
  Color color_;  ///< Color of the card (BLACK or RED).
  /** @} */      // End of CardLogic

  /**
   * @defgroup CardGUI Card GUI
   * @brief Variables related to the card's graphical representation.
   * @{
   */

  /**
   * @brief Return the bounding rectangle of the card item.
   *
   * This function defines the bounding area for the card, which is used
   * for collision detection.
   *
   * @return A QRectF object defining the bounding rectangle.
   */
  QRectF boundingRect() const override;

  /**
   * @brief Paint the card item on the scene.
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
   * @brief Handle mouse press events on the card.
   * @param event Pointer to the mouse event object.
   */
  void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

  /**
   * @brief Handle mouse move events on the card.
   * @param event Pointer to the mouse event object.
   */
  void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

  /**
   * @brief Handle mouse release events on the card.
   * @param event Pointer to the mouse event object.
   */
  void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

  /**
   * @brief Start the flipping animation.
   */
  void animateFlip();

  /**
   * @brief Get the flipping progress of the card.
   * @return qreal
   */
  qreal getFlipProgress() { return flipProgress_; }

  /**
   * @brief Set the flipping progress of the card.
   * @return qreal
   */
  void setFlipProgress(qreal progress) {
    flipProgress_ = progress;

    QTransform transform;
    double scale = progress / 90;
    double scaleHor = (progress < 90) ? 1 - scale : scale - 1;

    QPointF center = this->boundingRect().center();
    double scaledX = center.x() * this->scale();

    transform.translate(scaledX, 1);
    transform.scale(scaleHor, 1);
    transform.translate(-scaledX, 1);
    this->setTransform(transform);
  }

  /**
   * @brief Get the glow radius of the card.
   * @return qreal
   */
  qreal getGlowRadius() const { return glowEffect_->blurRadius(); }

  /**
   * @brief Set the glow radius of the card.
   * @param radius qreal
   */
  void setGlowRadius(qreal radius) { glowEffect_->setBlurRadius(radius); }

  QPixmap frontImage_;  ///< The front image of the card.
  QPixmap backImage_;   ///< The back image of the card.
  QPixmap pixmap_;      ///< Current pixmap (image).
  QGraphicsDropShadowEffect*
      glowEffect_;  ///< Drop shadow effect used for glowing the card.
  QPropertyAnimation* glowInAnimation_;   ///< Animation for glowing in.
  QPropertyAnimation* glowOutAnimation_;  ///< Animation for glowing out.
  QTimer* glowTimer_;  ///< Timer to handle glowing transitions.
  bool isGlowing_;     ///< Whether the card is currently glowing.
  QPropertyAnimation*
      moveAnimation_;     ///< Animation for moving the card to a new position.
  QPointF prevScenePos_;  ///< The previous scene position of the card.
  QPropertyAnimation* flipAnimation_;  ///< Animation for flipping the card.
  qreal flipProgress_;  ///< Progress of the flip animation (0 to 180).
  /** @} */             // End of CardGUI
};

#endif
