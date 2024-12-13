#include <QGuiApplication>
#include <catch2/catch_test_macros.hpp>

#include "card.hpp"
#include "qtTestApp.hpp"
#include "targetPile.hpp"

// Test-only class to expose protected methods
class TestTargetPile : public TargetPile {
 public:
  using TargetPile::addCard;
};

TEST_CASE_METHOD(QtTestApp, "TargetPile: Card Validity", "[targetPile]") {
  TestTargetPile pile;

  Card ace(Suit::HEARTS, Rank::ACE);
  ace.flip();

  Card two(Suit::HEARTS, Rank::TWO);
  two.flip();

  Card three(Suit::HEARTS, Rank::THREE);
  three.flip();

  Card invalidSuit(Suit::SPADES, Rank::TWO);
  invalidSuit.flip();

  SECTION("Empty pile: Only ACE of the correct suit is valid") {
    REQUIRE(pile.isValid(ace) == true);
    REQUIRE(pile.isValid(two) == false);
    REQUIRE(pile.isValid(invalidSuit) == false);

    // Add ACE to the pile
    pile.addCard(&ace);
    REQUIRE(pile.getTopCard() == &ace);
  }

  SECTION("Non-empty pile: Cards must be of the same suit and ascending rank") {
    // Add ACE to the pile
    pile.addCard(&ace);

    REQUIRE(pile.isValid(two) == true);
    REQUIRE(pile.isValid(three) == false);
    REQUIRE(pile.isValid(invalidSuit) == false);

    // Add TWO to the pile
    pile.addCard(&two);
    REQUIRE(pile.getTopCard() == &two);

    REQUIRE(pile.isValid(three) == true);
  }
}

TEST_CASE_METHOD(QtTestApp, "TargetPile: Visual Behavior", "[targetPile]") {
  TestTargetPile pile;

  Card* ace = new Card(Suit::DIAMONDS, Rank::ACE);
  Card* two = new Card(Suit::DIAMONDS, Rank::TWO);
  Card* three = new Card(Suit::DIAMONDS, Rank::THREE);

  ace->flip();
  two->flip();
  three->flip();

  pile.addCard(ace);
  pile.addCard(two);
  pile.addCard(three);

  SECTION("setCardsPrevScenePos updates positions correctly") {
    pile.setCardsPrevScenePos();

    REQUIRE(ace->getPrevScenePos() == QPointF(0, 0));
    REQUIRE(two->getPrevScenePos() ==
            QPointF(0, 0));  // No offset in TargetPile
    REQUIRE(three->getPrevScenePos() == QPointF(0, 0));
  }

  SECTION("updateVisuals positions all cards at the same location") {
    pile.updateVisuals();

    REQUIRE(ace->pos() == QPointF(0, 0));
    REQUIRE(two->pos() == QPointF(0, 0));
    REQUIRE(three->pos() == QPointF(0, 0));
  }
}
