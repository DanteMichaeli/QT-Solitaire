#include <QGuiApplication>
#include <catch2/catch_test_macros.hpp>

#include "deck.hpp"
#include "wastePile.cpp"
#include "wastePile.hpp"

TEST_CASE("WastePile Cards Only Added Through AddFromDeck", "[wastepile]") {
  int argc = 1;
  char* argv[] = {""};
  QGuiApplication app(argc, argv);  // Initialize QGuiApplication

  Deck deck;
  WastePile pile;

  // loop through all cards in deck, and check isValid for each one
  for (int i = 0; i < 52; i++) {
    auto card = deck.RemoveCard();
    REQUIRE(pile.isValid(*card) == false);
  }
}

TEST_CASE("WastePile Working AddFromDeck and AddToDeck", "[wastepile]") {
  int argc = 1;
  char* argv[] = {""};
  QGuiApplication app(argc, argv);  // Initialize QGuiApplication

  Deck deck;
  WastePile pile;

  // move 3 cards from deck to waste pile
  pile.AddFromDeck(deck, 3);
  REQUIRE(pile.getCards().size() == 3);

  // empty deck
  while (!deck.Empty()) {
    deck.RemoveCard();
  }

  // move all cards from waste pile back to deck
  bool success = pile.AddToDeck(deck);
  REQUIRE(success == true);
  REQUIRE(pile.getCards().size() == 0);
}
