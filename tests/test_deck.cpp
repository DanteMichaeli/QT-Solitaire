#include <QGuiApplication>
#include <catch2/catch_test_macros.hpp>

#include "deck.cpp"
#include "deck.hpp"

TEST_CASE("Deck Initialization", "[deck]") {
  int argc = 1;
  char *argv[] = {""};
  QGuiApplication app(argc, argv);

  Deck deck;
  REQUIRE(deck.Empty() ==
          false);  // Deck should not be empty after initialization
}

TEST_CASE("Deck Shuffle", "[deck]") {
  int argc = 1;
  char *argv[] = {""};
  QGuiApplication app(argc, argv);

  Deck deck;
  deck.Shuffle();
  // Since shuffle is random, we can't predict the order, but we can check if
  // the deck is still not empty
  REQUIRE(deck.Empty() == false);
}

TEST_CASE("Deck Peek", "[deck]") {
  int argc = 1;
  char *argv[] = {""};
  QGuiApplication app(argc, argv);

  Deck deck;
  REQUIRE_NOTHROW(deck.Peek());  // Should not throw an exception
}

TEST_CASE("Deck Deal Card", "[deck]") {
  int argc = 1;
  char *argv[] = {""};
  QGuiApplication app(argc, argv);

  Deck deck;
  auto card = deck.dealCard();
  REQUIRE(card != nullptr);  // Should return a card
  REQUIRE(deck.Empty() ==
          false);  // Deck should not be empty after dealing one card
}

TEST_CASE("Deck Empty Check", "[deck]") {
  int argc = 1;
  char *argv[] = {""};
  QGuiApplication app(argc, argv);

  Deck deck;
  while (!deck.Empty()) {
    deck.dealCard();
  }
  REQUIRE(deck.Empty() ==
          true);  // Deck should be empty after dealing all cards
}

TEST_CASE("Deck Add Card", "[deck]") {
  int argc = 1;
  char *argv[] = {""};
  QGuiApplication app(argc, argv);

  Deck deck;
  auto card = std::make_unique<Card>(Suit::HEARTS, Rank::ACE);
  deck.AddCard(std::move(card));
  REQUIRE(deck.Empty() ==
          false);  // Deck should not be empty after adding a card
}