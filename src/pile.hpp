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

  int Size() const { return cards_.size(); }

  bool Empty() const { return cards_.empty(); }

  Card* getTopCard() const;

  vector<unique_ptr<Card>>& getCards() { return cards_; }

  int cardIndexFromBack(Card* card) const;

  /**
   * @brief Add a card to the pile.
   * @param card A unique pointer to the Card to add.
   */
  void AddCard(std::unique_ptr<Card>& card);

  /**
   * @brief Removes and returns the top card from the pile.
   * @return A unique pointer to the top Card object, or nullptr if the pile
   * is empty.
   */
  unique_ptr<Card> RemoveCard();

  /**
   * @brief A function that moves a card to another pile.
   * @param other The target pile to which cards are moved to.
   * @return true if the card is successfully moved, false otherwise.
   */
  void TransferCard(Pile& other, size_t nof = 1);

  /**
   * @brief Pure virtual function to check if a card can be added to the pile.
   *        Must be implemented by derived classes.
   * @param card The card to check.
   * @return true if the card can be added, false otherwise.
   */
  virtual bool IsValid(const Card& card) = 0;

  virtual void updateVisuals() = 0;  // Refresh the appearance of the pile

 signals:
  void cardMoved(Card* card, Pile* fromPile, QPointF newScenePos);
  void cardClickMove(Card* card, Pile* fromPile);

 protected:
  QRectF boundingRect() const override;

  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
             QWidget* widget = nullptr) override;

  /**
   * @brief Collection of unique pointers to Card objects in the pile.
   */
  vector<unique_ptr<Card>> cards_;

 private slots:
  void onCardClicked(Card* card);
  void onCardDragged(Card* card, const QPointF& newScenePos);

 private:
  QRectF rect_;
};

#endif
