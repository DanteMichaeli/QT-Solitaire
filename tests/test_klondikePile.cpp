#include <QGuiApplication>
#include <catch2/catch_test_macros.hpp>

#include "card.hpp"
#include "klondikePile.hpp"
#include "qtTestApp.hpp"

class TestKlondikePile : public KlondikePile {
 public:
  using KlondikePile::addCard;
};

TEST_CASE_METHOD(QtTestApp, "KlondikePile: Card Validity", "[klondikePile]") {
  TestKlondikePile pile;

  Card king(Suit::SPADES, Rank::KING);
  king.flip();

  Card queen(Suit::HEARTS, Rank::QUEEN);
  queen.flip();

  Card invalidCard(Suit::SPADES, Rank::QUEEN);
  invalidCard.flip();

  SECTION("Empty pile: Only KING is valid") {
    REQUIRE(pile.isValid(king) == true);

    // Add KING to the pile using the test-only addCard
    pile.addCard(&king);

    REQUIRE(pile.getTopCard() == &king);
  }

  SECTION("Non-empty pile: Valid sequence of cards") {
    // Add KING first
    pile.addCard(&king);
    REQUIRE(pile.getTopCard() == &king);

    // Check QUEEN validity
    REQUIRE(pile.isValid(queen) == true);

    // Add QUEEN to the pile
    pile.addCard(&queen);
    REQUIRE(pile.getTopCard() == &queen);

    // Invalid card (same color) cannot be added
    REQUIRE(pile.isValid(invalidCard) == false);
  }
}

TEST_CASE_METHOD(QtTestApp, "KlondikePile: Visual Behavior", "[klondikePile]") {
  TestKlondikePile pile;

  Card* card1 = new Card(Suit::SPADES, Rank::KING);
  Card* card2 = new Card(Suit::HEARTS, Rank::QUEEN);
  Card* card3 = new Card(Suit::CLUBS, Rank::JACK);

  card1->flip();
  card2->flip();
  card3->flip();

  // Add cards to the pile
  pile.addCard(card1);
  pile.addCard(card2);
  pile.addCard(card3);

  SECTION("setCardsPrevScenePos updates positions correctly") {
    pile.setCardsPrevScenePos();

    REQUIRE(card1->getPrevScenePos() == QPointF(0, 0));
    REQUIRE(card2->getPrevScenePos().y() > card1->getPrevScenePos().y());
    REQUIRE(card3->getPrevScenePos().y() > card2->getPrevScenePos().y());
  }
}