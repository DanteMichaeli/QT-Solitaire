#include "game.hpp"

#include <exception>

Game::Game(QObject* parent)
    : klondikePiles_(KLONDIKE_PILE_AM),
      targetPiles_(TARGET_PILE_AM),
      QObject(parent) {
  initGame();
}

void Game::initGame() {
  deck_ = std::make_unique<Deck>();
  deck_->Shuffle();
  connect(deck_.get(), &Deck::deckClicked, this, &Game::deckClicked);
  wastePile_ = std::make_unique<WastePile>();
  for (int i = 0; i < KLONDIKE_PILE_AM; i++) {
    if (deck_->Empty()) {
      throw std::out_of_range("pile empty");
    }
    klondikePiles_[i] = std::make_unique<KlondikePile>(*deck_, i + 1, 1);
  }
  for (size_t i = 0; i < TARGET_PILE_AM; ++i) {
    targetPiles_[i] = std::make_unique<TargetPile>(allSuits[i]);
  }
}

void Game::deckClicked() {
  if (!deck_->Empty()) {
    wastePile_->AddFromDeck(*deck_, 1);
    wastePile_->updateVisuals();
  } else {
    wastePile_->AddToDeck(*deck_, false);
  }
}