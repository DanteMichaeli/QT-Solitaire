#ifndef PILE_HPP
#define PILE_HPP

#include <QGraphicsObject>
#include <QPainter>

#include "card.hpp"

#define PILE_OFFSET 20

using namespace std;

/**
 * @class Pile
 * @brief Represents a pile of cards with the ability to add, remove, and
 * transfer cards. This is an abstract base class.
 */
class Pile : public QGraphicsObject {
  Q_OBJECT
  Q_INTERFACES(QGraphicsItem)
 public:
  /**
   * @defgroup PileLogic Pile Logic
   * @brief Members related to the pile's logical state.
   * @{
   */

  /**
   * @brief Construct an empty pile.
   * @param parent Pointer to the parent QGraphicsItem, if any.
   */
  explicit Pile(QGraphicsItem* parent = nullptr);

  /**
   * @brief Destruct Pile.
   */
  virtual ~Pile();

  /**
   * @brief Get the number of cards in the pile.
   * @return The number of cards in the pile.
   */
  size_t getSize() const { return cards_.size(); }

  /**
   * @brief Check whether the pile container is empty.
   * @return true if the pile is empty, false otherwise.
   */
  bool isEmpty() const { return cards_.empty(); }

  /**
   * @brief Get the pointer to the card that is on top of the pile.
   * @return Pointer to the top card if the pile is not empty, otherwise
   * nullptr.
   */
  Card* getTopCard() const;

  /**
   * @brief Get a card from the back of the pile by index.
   * @param i The index of the card from the back.
   * @return Pointer to the specified card.
   */
  Card* getCardFromBack(const size_t i);

  /**
   * @brief Get the index of a card from the top of the pile.
   *
   * Index starts from 1, representing the subpile that consists of this card
   * and all cards above it.
   *
   * @param card Pointer to the card to find.
   * @return The index of the card in the pile, or 0 if the card is not found.
   */
  int cardIndexFromTop(Card* card) const;

  /**
   * @brief Move one or more card from this pile to another pile.
   * @param other Reference to the target pile.
   * @param nof Number of cards to transfer (default is 1).
   */
  void transferCards(Pile& other, const unsigned int nof = 1);

  /**
   * @brief Get all cards above a specific card in the pile.
   * @param card Pointer to the card in the pile.
   * @return A vector of pointers to the cards above the specified card.
   */
  vector<Card*> getCardsAbove(Card* card) const;

  /**
   * @brief Flip the top card up/down.
   *
   * In game class, if flipping is succesful, adds points for the player.
   *
   * @param value Boolean value. True for flip up, false for down.
   * @return true when card is flipped up.
   * @return false otherwise.
   */
  bool flipTopCard(bool faceUp, const int indexFromBack = 1);

  /**
   * @brief Check if a card can be added to the pile. Pure virtual function.
   * @param card The card to check.
   * @return true if the card can be added, false otherwise.
   */
  virtual bool isValid(const Card& card) = 0;

  /** @} */  // End of PileLogic

  /**
   * @defgroup PileGUI Pile GUI
   * @brief Members related to the pile's graphical representation.
   * @{
   */

  /**
   * @brief Get the unscaled width of the pile.
   * @return The unscaled width.
   */
  double getWidth() const { return rect_.width(); }

  /**
   * @brief Get the unscaled height of the pile.
   * @return The unscaled height.
   */
  double getHeight() const { return rect_.height(); }

  /**
   * @brief Update the visual representation of the pile. Pure virtual function.
   *
   */
  virtual void updateVisuals() = 0;

  /**
   * @brief Set the previous scene positions of all cards in the pile. Pure
   * virtual function.
   */
  virtual void setCardsPrevScenePos() = 0;

  /**
   * @brief Get the offset used for positioning cards in the pile. Pure virtual
   * function.
   * @return The offset as a QPointF.
   */
  virtual QPointF getOffset() const = 0;

 signals:
  /**
   * @brief Signal emitted when a card is moved.
   * @param card Pointer to the card being moved.
   * @param fromPile Pointer to the originating pile.
   * @param newScenePos The new position of the card in scene coordinates.
   */
  void cardMoved(Card* card, Pile* fromPile, const QPointF& newScenePos);

  /**
   * @brief Signal emitted when a card is clicked to initiate a move.
   * @param card Pointer to the card being moved.
   * @param fromPile Pointer to the originating pile.
   */
  void cardClickMove(Card* card, Pile* fromPile);

  /** @} */  // End of PileGUI

 protected:
  /**
   * @addtogroup PileLogic
   * @{
   */

  vector<Card*> cards_;  ///< All the cards inside this pile.

  /**
   * @brief Add a card to the pile.
   * @param card Pointer to the card to be added.
   */
  void addCard(Card* card);

  /** @} */  // End of PileLogic

  /**
   * @addtogroup PileGUI
   * @{
   */

  const QRectF rect_;  ///< The rectangle defining the item’s graphical size.

  /**
   * @brief Return the bounding rectangle of the item. Defines the area within
   * which the item can be drawn and interacted with.
   *
   * @return A QRectF object specifying the bounding rectangle.
   */
  QRectF boundingRect() const override;

 private slots:
  /**
   * @brief Slot called when a card is clicked.
   * @param card Pointer to the clicked card.
   */
  void onCardClicked(Card* card);

  /**
   * @brief Slot called when a card is dragged.
   * @param card Pointer to the dragged card.
   * @param newScenePos The new position of the card in scene coordinates.
   */
  void onCardDragged(Card* card, const QPointF& newScenePos);

  /** @} */  // End of PileGUI

 private:
  /**
   * @addtogroup PileLogic
   * @{
   */

  /**
   * @brief Remove and return the top card from the pile.
   * @return Pointer to the top card, or nullptr if the pile is empty.
   */
  Card* removeCard();

  /** @} */  // End of PileLogic

  /**
   * @addtogroup PileGUI
   * @{
   */

  /**
   * @brief Paint the pile slot on the scene.
   * @param painter Pointer to the QPainter used for drawing.
   * @param option Provides style options for the item.
   * @param widget Optional pointer to the widget being painted on.
   */
  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
             QWidget* widget = nullptr) override;

  /** @} */  // End of PileGUI
};

#endif
