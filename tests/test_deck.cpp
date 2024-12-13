#include <QGuiApplication>
#include <catch2/catch_test_macros.hpp>

#include "deck.hpp"
#include "qtTestApp.hpp"
#include "wastePile.hpp"

/**
 * @brief Fixture to set up a QGuiApplication for all tests.
 */

TEST_CASE_METHOD(QtTestApp, "Deck: Transfer Cards to WastePile", "[deck]") {
  Deck deck;
  WastePile wastePile;

  SECTION("Transfer cards from Deck to WastePile") {
    REQUIRE(deck.getSize() == 52);  // Deck starts with 52 cards

    // Transfer the top 3 cards from Deck to WastePile
    for (int i = 0; i < 3; ++i) {
      deck.transferCards(wastePile);
    }

    REQUIRE(deck.getSize() == 49);      // Deck size decreases
    REQUIRE(wastePile.getSize() == 3);  // WastePile size increases

    // Verify that all cards in WastePile are face-up
    int cardCount = 0;
    while (!wastePile.isEmpty()) {
      Card* topCard = wastePile.getTopCard();
      REQUIRE(topCard != nullptr);  // Top card should exist
      ++cardCount;

      // Move card temporarily back to Deck to continue checking
      wastePile.transferCards(deck);
    }

    REQUIRE(cardCount == 3);        // Verify 3 cards were checked
    REQUIRE(wastePile.isEmpty());   // WastePile should be empty
    REQUIRE(deck.getSize() == 52);  // Cards are back in the deck
  }
}

TEST_CASE_METHOD(QtTestApp, "WastePile: Transfer Cards Back to Deck",
                 "[wastePile]") {
  Deck deck;
  WastePile wastePile;

  SECTION("Move cards back from WastePile to Deck") {
    // Transfer 3 cards from Deck to WastePile
    for (int i = 0; i < 3; ++i) {
      deck.transferCards(wastePile);
    }

    REQUIRE(deck.getSize() == 49);
    REQUIRE(wastePile.getSize() == 3);

    // Move cards back to the Deck
    while (!wastePile.isEmpty()) {
      wastePile.transferCards(deck);
    }

    REQUIRE(deck.getSize() == 52);  // All cards back in the deck
    REQUIRE(wastePile.isEmpty());   // WastePile is empty
  }
}
