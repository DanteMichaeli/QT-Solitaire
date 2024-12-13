#include <QGuiApplication>
#include <catch2/catch_test_macros.hpp>

#include "card.hpp"
#include "game.hpp"
#include "qtTestApp.hpp"

TEST_CASE_METHOD(QtTestApp, "Game Initialization", "[game]") {
  Game game;
  game.startGame();

  REQUIRE(game.getDeck() != nullptr);
  REQUIRE(game.getWPile() != nullptr);
  REQUIRE(game.getKPiles().size() == 7);
  REQUIRE(game.getTPiles().size() == 4);

  REQUIRE(game.getDeck()->getSize() == 24);
  REQUIRE(game.getWPile()->isEmpty() == true);
}

TEST_CASE_METHOD(QtTestApp, "Game Moves and Scoring", "[game]") {
  Game game;
  game.startGame();

  Deck* deck = game.getDeck();
  WastePile* waste = game.getWPile();

  SECTION("Deck to WastePile Move") {
    int points = game.attemptDeckMove();
    REQUIRE(deck->getSize() == 21);
    REQUIRE(waste->getSize() == 3);
  }

  SECTION("WastePile to KlondikePile Move") {
    game.attemptDeckMove();
    Card* topCard = waste->getTopCard();

    Pile* klondikePile = game.getKPiles()[0];

    if (klondikePile->isValid(*topCard)) {
      int points = game.attemptMove(topCard, waste, klondikePile);
      REQUIRE(points == Game::wToKPoints);
      REQUIRE(waste->getSize() == 2);
      REQUIRE(klondikePile->getTopCard() == topCard);
    }
  }

  SECTION("Undo Last Move") {
    game.attemptDeckMove();
    REQUIRE(waste->getSize() == 3);
    game.undo();
    REQUIRE(deck->getSize() == 21);
  }
}

class TestTargetPile : public TargetPile {
 public:
  using TargetPile::addCard;
};

TEST_CASE_METHOD(QtTestApp, "Game Win Condition with Existing TargetPiles",
                 "[game]") {
  Game game;
  game.startGame();

  auto targetPiles = game.getTPiles();
  REQUIRE(targetPiles.size() == 4);

  // Temporary pile to simulate card transfers
  TestTargetPile tempPile;

  // Add cards ACE to KING for all suits to each TargetPile
  for (int suitIndex = 0; suitIndex < 4; ++suitIndex) {
    Suit suit = static_cast<Suit>(suitIndex);
    TargetPile* targetPile = targetPiles[suitIndex];

    for (int rank = Rank::ACE; rank <= Rank::KING; ++rank) {
      Card* card = new Card(suit, static_cast<Rank>(rank));
      card->flip();

      // Simulate transferring cards into the TargetPile
      tempPile.addCard(card);
      tempPile.transferCards(*targetPile);
    }

    REQUIRE(targetPile->getSize() == 13);
  }

  // Verify the game recognizes the win condition
  REQUIRE(game.hasWon() == true);
}
