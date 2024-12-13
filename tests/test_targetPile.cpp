#include <QGuiApplication>
#include <catch2/catch_test_macros.hpp>

#include "deck.hpp"
#include "klondikePile.hpp"
#include "targetPile.cpp"
#include "targetPile.hpp"

TEST_CASE("TargetPile Check isValid Check with Loop", "[targetpile]") {
  int argc = 1;
  char* argv[] = {""};
  QGuiApplication app(argc, argv);  // Initialize QGuiApplication

  TargetPile pile(Suit::HEARTS);
  Deck deck;

  // Wrong suit cannot be added
  auto card = std::make_unique<Card>(Suit::SPADES, Rank::ACE);
  card->flipUp();
  REQUIRE(pile.isValid(*card) == false);

  // Correct suit but wrong rank
  card = std::make_unique<Card>(Suit::HEARTS, Rank::TWO);
  card->flipUp();
  REQUIRE(pile.isValid(*card) == false);

  KlondikePile kPile(deck);  // Create a KlondikePile for transferring cards

  // Loop through all ranks (1 to 13) and add them to the target pile
  for (int rank = 1; rank <= 13; ++rank) {
    auto correctCard =
        std::make_unique<Card>(Suit::HEARTS, static_cast<Rank>(rank));
    correctCard->flipUp();

    auto wrongCard =
        std::make_unique<Card>(Suit::SPADES, static_cast<Rank>(rank));
    wrongCard->flipUp();

    REQUIRE(pile.isValid(*wrongCard) == false);  // Wrong suit should fail
    REQUIRE(pile.isValid(*correctCard) ==
            true);  // Correct suit and rank should pass

    // Add the correct card to the KlondikePile, then transfer it to the
    // TargetPile
    kPile.AddCard(correctCard);
    REQUIRE(kPile.TransferCard(pile) == true);  // Ensure successful transfer
  }
}