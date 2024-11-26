#include <QGuiApplication>
#include <catch2/catch_test_macros.hpp>

#include "deck.hpp"
#include "klondikePile.cpp"
#include "klondikePile.hpp"
#include "pile.cpp"
#include "pile.hpp"

TEST_CASE("KlondikePile Initialization", "[klondikepile]") {
  int argc = 1;
  char* argv[] = {""};
  QGuiApplication app(argc, argv);  // Initialize QGuiApplication

  // Initialize a deck and create a KlondikePile with 5 cards, 3 face up
  Deck deck;
  KlondikePile pile(deck, 5, 3);

  // Verify the pile is not empty
  REQUIRE(pile.getCards().size() == 5);
  REQUIRE(pile.getCards().back()->isFaceUp() ==
          true);  // Last card (top of pile) should be face up
}

TEST_CASE("KlondikePile Add Card", "[klondikepile]") {
  int argc = 1;
  char* argv[] = {""};
  QGuiApplication app(argc, argv);  // Initialize QGuiApplication

  Deck deck;
  KlondikePile pile(deck, 5, 3);

  // Create a new card and try adding it to the pile
  auto newCard = std::make_unique<Card>(Suit::SPADES, Rank::QUEEN);
  pile.AddCard(newCard);

  REQUIRE(pile.getCards().size() == 6);  // Pile should now have 6 cards
}

TEST_CASE("KlondikePile Remove Card", "[klondikepile]") {
  int argc = 1;
  char* argv[] = {""};
  QGuiApplication app(argc, argv);  // Initialize QGuiApplication

  Deck deck;
  KlondikePile pile(deck, 5, 3);

  // Remove a card from the pile
  auto removedCard = pile.RemoveCard();
  REQUIRE(removedCard != nullptr);       // Should return a card
  REQUIRE(pile.getCards().size() == 4);  // Pile should have 4 cards left
}

TEST_CASE("KlondikePile IsValid Function", "[klondikepile]") {
  int argc = 1;
  char* argv[] = {""};
  QGuiApplication app(argc, argv);  // Initialize QGuiApplication

  Deck deck;
  KlondikePile pile(deck, 0, 0);  // Start with an empty pile

  // Case 1: Valid move to an empty pile (KING)
  auto king = std::make_unique<Card>(Suit::HEARTS, Rank::KING);
  king->flipUp();  // Card must be face up
  REQUIRE(pile.IsValid(*king) == true);

  // Case 2: Invalid move to an empty pile (non-KING)
  auto queen = std::make_unique<Card>(Suit::DIAMONDS, Rank::QUEEN);
  queen->flipUp();
  REQUIRE(pile.IsValid(*queen) == false);

  // Add the KING to the pile
  pile.AddCard(king);

  // Case 3: Valid move to a non-empty pile
  auto validCard = std::make_unique<Card>(Suit::SPADES, Rank::QUEEN);
  validCard->flipUp();
  REQUIRE(pile.IsValid(*validCard) == true);

  // Case 4: Invalid move to a non-empty pile (same color)
  auto invalidCard1 = std::make_unique<Card>(Suit::HEARTS, Rank::QUEEN);
  invalidCard1->flipUp();
  REQUIRE(pile.IsValid(*invalidCard1) == false);

  // Case 5: Invalid move to a non-empty pile (not one rank lower)
  auto invalidCard2 = std::make_unique<Card>(Suit::SPADES, Rank::TEN);
  invalidCard2->flipUp();
  REQUIRE(pile.IsValid(*invalidCard2) == false);

  // Case 6: Invalid move with a face-down card
  auto faceDownCard = std::make_unique<Card>(Suit::CLUBS, Rank::QUEEN);
  REQUIRE(pile.IsValid(*faceDownCard) == false);
}

TEST_CASE("KlondikePile TransferSubPile Success", "[klondikepile]") {
  int argc = 1;
  char* argv[] = {""};
  QGuiApplication app(argc, argv);  // Initialize QGuiApplication

  // Initialize source and destination piles with a deck
  Deck deck;
  KlondikePile sourcePile(deck, 0, 0);  // Empty source pile
  KlondikePile destPile(deck, 0, 0);    // Empty destination pile

  // Case 1: Valid transfer
  auto card1 = std::make_unique<Card>(Suit::SPADES, Rank::KING);
  card1->flipUp();
  sourcePile.AddCard(card1);

  auto card2 = std::make_unique<Card>(Suit::HEARTS, Rank::QUEEN);
  card2->flipUp();
  sourcePile.AddCard(card2);

  REQUIRE(sourcePile.TransferSubPile(destPile, 2) == true);
  REQUIRE(sourcePile.getCards().size() == 0);  // Source pile should be empty
  REQUIRE(destPile.getCards().size() ==
          2);  // Destination pile should have 2 cards
}

TEST_CASE("KlondikePile TransferSubPile Failure", "[klondikepile]") {
  int argc = 1;
  char* argv[] = {""};
  QGuiApplication app(argc, argv);  // Initialize QGuiApplication

  // Initialize source and destination piles with a deck
  Deck deck;
  KlondikePile sourcePile(deck, 0, 0);  // Empty source pile
  KlondikePile destPile(deck, 0, 0);    // Empty destination pile

  // Add an invalid card to the destination pile
  auto card1 = std::make_unique<Card>(Suit::HEARTS, Rank::QUEEN);
  card1->flipUp();
  destPile.AddCard(card1);  // Add a Queen to the destination pile

  // Add a card sequence to the source pile
  auto card2 = std::make_unique<Card>(Suit::SPADES, Rank::KING);
  card2->flipUp();
  sourcePile.AddCard(card2);

  auto card3 = std::make_unique<Card>(Suit::CLUBS, Rank::QUEEN);
  card3->flipUp();
  sourcePile.AddCard(card3);

  // Attempt to transfer the source pile to the destination pile
  // This should fail because a QUEEN cannot be placed on another QUEEN
  REQUIRE(sourcePile.TransferSubPile(destPile, 2) == false);
  REQUIRE(sourcePile.getCards().size() ==
          2);  // Source pile should remain unchanged
  REQUIRE(destPile.getCards().size() ==
          1);  // Destination pile should remain unchanged
}