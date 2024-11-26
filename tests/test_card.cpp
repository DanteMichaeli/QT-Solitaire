#include <QGuiApplication>  // Include the necessary Qt header for QGuiApplication
#include <catch2/catch_test_macros.hpp>

#include "card.hpp"

TEST_CASE("Card Initialization", "[card]") {
  // Create the application context for Qt
  int argc = 1;
  char *argv[] = {""};
  QGuiApplication app(argc, argv);  // Initialize QGuiApplication

  // Test initialization of Card with Suit::HEARTS and Rank::ACE
  Card card(Suit::HEARTS, Rank::ACE);

  // Verify suit and rank are correctly set
  REQUIRE(card.GetSuit() == Suit::HEARTS);
  REQUIRE(card.GetRank() == Rank::ACE);
  REQUIRE(card.GetColor() == Color::RED);
  REQUIRE(card.isFaceUp() == false);  // Card should be face-down initially
}

TEST_CASE("Card Flip Up and Flip Down", "[card]") {
  // Create the application context for Qt
  int argc = 1;
  char *argv[] = {""};
  QGuiApplication app(argc, argv);  // Initialize QGuiApplication

  // Test flipUp and flipDown functionality
  Card card(Suit::SPADES, Rank::KING);

  card.flipUp();
  REQUIRE(card.isFaceUp() == true);  // The card should now be face-up

  card.flipDown();
  REQUIRE(card.isFaceUp() == false);  // The card should be face-down again
}

TEST_CASE("Card String Representation", "[card]") {
  // Create the application context for Qt
  int argc = 1;
  char *argv[] = {""};
  QGuiApplication app(argc, argv);  // Initialize QGuiApplication

  // Test the string representation of a card
  Card card(Suit::DIAMONDS, Rank::JACK);

  // Expected string format: "JACK_of_DIAMONDS"
  REQUIRE(card.cardToQString() == "jack_of_diamonds");
}