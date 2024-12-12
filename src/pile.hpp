#ifndef PILE_HPP
#define PILE_HPP

#include <QGraphicsObject>
#include <QPainter>
#include <memory>

#include "card.hpp"

#define PILE_YOFFSET 20

using namespace std;

/**
 * @class Pile
 * @brief Represents a pile of cards with the ability to add, remove, and
 * transfer cards. This is an abstract base class, as it includes a pure virtual
 * method IsValid.
 */
class Pile : public QGraphicsObject {
  Q_OBJECT
  Q_INTERFACES(QGraphicsItem)
 public:
  /**
   * @brief Default constructor for an empty pile.
   */
  explicit Pile(QGraphicsItem* parent = nullptr);

  /**
   * @brief Destructor for the Pile, outputs a message when the pile is
   * destroyed.
   */
  virtual ~Pile();

  /**
   * @brief Get the size of pile container.
   * @return point value of size.
   */
  int Size() const { return cards_.size(); }

  /**
   * @brief Check whether pile container is empty.
   * @return true if empty.
   * @return false otherwise.
   */
  bool Empty() const { return cards_.empty(); }

  /**
   * @brief Get the pointer to the card that is on top of teh pile.
   * @return Card pointer if pile is not empty, otherwise nullptr.
   */
  Card* getTopCard() const;

  Card* getCardFromBack(size_t i);

  const vector<Card*>& getCards() const { return cards_; }
  /**
   * @brief Get the index of a card, going from top to bottom.
   *
   * Index starts from 1 as this represents how many cards are in the subpile
   * that constsit of this card and all the cards on top of it.
   *
   * @param card to be searched for.
   * @return size of subpile or 0 if card is not found.
   */
  int cardIndexFromBack(Card* card) const;

  /**
   * @brief Add a card to the pile.
   * @param card A unique pointer to the Card to add.
   */
  void AddCard(Card* card);

  /**
   * @brief Removes and returns the top card from the pile.
   * @return A unique pointer to the top Card object, or nullptr if the pile
   * is empty.
   */
  Card* RemoveCard();

  /**
   * @brief A function that moves a card to another pile.
   * @param other The target pile to which cards are moved to.
   * @return true if the card is successfully moved, false otherwise.
   */
  void TransferCard(Pile& other, size_t nof = 1);

  vector<Card*> getCardsAbove(Card* card);

  double getScaledWidth() { return rect_.width() * this->scale(); }
  double getScaledHeigh() { return rect_.height() * this->scale(); }
  double getWidth() { return rect_.width(); }
  double getHeigh() { return rect_.height(); }

  /**
   * @brief Pure virtual function to check if a card can be added to the pile.
   *        Must be implemented by derived classes.
   * @param card The card to check.
   * @return true if the card can be added, false otherwise.
   */
  virtual bool IsValid(const Card& card) = 0;

  virtual void updateVisuals() = 0;  // Refresh the appearance of the pile

  virtual void setCardPrevScenePos() = 0;

  virtual QPointF getOffset() const = 0;

 signals:
  /**
   * @brief Signal emitted when a card is moved.
   *
   * This signal is triggered when a card is moved to a new position within the
   * scene.
   *
   * @param card Pointer to the card being moved.
   * @param fromPile Pointer to the originating pile.
   * @param newScenePos The new position of the card in scene coordinates.
   */
  void cardMoved(Card* card, Pile* fromPile, QPointF newScenePos);

  /**
   * @brief Signal emitted when a card is clicked to initiate a move.
   *
   * This signal is triggered when a card is clicked and a move is initiated
   * based on the click interaction.
   *
   * @param card Pointer to the card being moved.
   * @param fromPile Pointer to the originating pile.
   */
  void cardClickMove(Card* card, Pile* fromPile);

 protected:
  /**
   * @brief Returns the bounding rectangle of the item.
   *
   * This function defines the area within which the item can be drawn and
   * interacted with.
   *
   * @return A QRectF object specifying the bounding rectangle.
   */
  QRectF boundingRect() const override;

  /**
   * @brief Paints the pile slot on the scene.
   * @param painter Pointer to the QPainter used for drawing.
   * @param option Provides style options for the item.
   * @param widget Optional pointer to the widget being painted on.
   */
  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
             QWidget* widget = nullptr) override;

  vector<Card*> cards_;  ///< All the cards inside this pile.
  QRectF rect_;          ///< The rectangle defining the item’s boundaries.

 private slots:
  /**
   * @brief Slot called when a card is clicked.
   *
   * Handles the logic for when a card is clicked within the scene.
   *
   * @param card Pointer to the clicked card.
   */
  void onCardClicked(Card* card);

  /**
   * @brief Slot called when a card is dragged.
   *
   * Handles the logic for when a card is dragged to a new position in the
   * scene.
   *
   * @param card Pointer to the dragged card.
   * @param newScenePos The new position of the card in scene coordinates.
   */
  void onCardDragged(Card* card, const QPointF& newScenePos);
};

#endif
