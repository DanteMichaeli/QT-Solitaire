#include <QGuiApplication>
#include <catch2/catch_test_macros.hpp>

#include "card.hpp"
#include "qtTestApp.hpp"

QGuiApplication* QtTestApp::app = nullptr;  // Definition
TEST_CASE_METHOD(QtTestApp, "Card Initialization Test", "[card]") {
  Card card(Suit::HEARTS, Rank::ACE);

  SECTION("Card suit and rank") {
    REQUIRE(card.getSuit() == Suit::HEARTS);
    REQUIRE(card.getRank() == Rank::ACE);
  }

  SECTION("Card default face-up state") { REQUIRE(card.isFaceUp() == false); }

  SECTION("Card color for HEARTS is RED") {
    REQUIRE(card.getColor() == Color::RED);
  }
}

TEST_CASE_METHOD(QtTestApp, "Card Flip Functionality", "[card]") {
  Card card(Suit::SPADES, Rank::KING);

  SECTION("Flip the card face-up") {
    card.flip();
    REQUIRE(card.isFaceUp() == true);
  }

  SECTION("Flip the card face-down") {
    card.flip();
    card.flip();
    REQUIRE(card.isFaceUp() == false);
  }
}

TEST_CASE_METHOD(QtTestApp, "Card String Representation", "[card]") {
  Card card(Suit::DIAMONDS, Rank::JACK);

  SECTION("String representation matches expected format") {
    REQUIRE(card.cardToQString() == "jack_of_diamonds");
  }
}
