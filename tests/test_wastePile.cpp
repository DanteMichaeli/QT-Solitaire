#include <QGuiApplication>
#include <catch2/catch_test_macros.hpp>

#include "card.hpp"
#include "deck.hpp"
#include "qtTestApp.hpp"
#include "wastePile.hpp"

TEST_CASE_METHOD(QtTestApp, "WastePile: Draw Cards from Deck", "[wastePile]") {
  Deck deck;
  WastePile waste;

  SECTION("Draw cards from the deck") {
    REQUIRE(deck.getSize() == 52);
    REQUIRE(waste.getSize() == 0);

    int drawnCards = waste.addFromDeck(deck, 3);

    REQUIRE(drawnCards == 3);
    REQUIRE(deck.getSize() == 49);
    REQUIRE(waste.getSize() == 3);

    for (int i = 0; i < 3; ++i) {
      Card* topCard = waste.getTopCard();
      REQUIRE(topCard != nullptr);
      REQUIRE(topCard->isFaceUp() == true);
      waste.transferCards(deck);
    }
    REQUIRE(waste.getSize() == 0);
    REQUIRE(deck.getSize() == 52);
  }
}

TEST_CASE_METHOD(QtTestApp, "WastePile: Visual Behavior", "[wastePile]") {
  Deck deck;
  WastePile waste;

  waste.addFromDeck(deck, 3);

  REQUIRE(waste.getSize() == 3);

  SECTION("updateVisuals maintains stacking order") {
    waste.updateVisuals();

    // Verify that all cards visually appear stacked at the same position
    QPointF expectedPos = QPointF(0, 0);
    for (int i = 0; i < 3; ++i) {
      Card* card = waste.getTopCard();
      REQUIRE(card->pos() == expectedPos);
      waste.transferCards(deck);
    }
  }
}
