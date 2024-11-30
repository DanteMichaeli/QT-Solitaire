#include "game.hpp"

#include <exception>

Game::Game(QObject* parent)
    : klondikePiles_(KLONDIKE_PILE_AM),
      targetPiles_(TARGET_PILE_AM),
      QObject(parent) {
  initDeck();
  initWastePile();
  initKlondikePiles();
  initTargetPiles();
}

void Game::initDeck() {
  deck_ = std::make_unique<Deck>();
  deck_->Shuffle();
  connect(deck_.get(), &Deck::deckClicked, this, &Game::deckClicked);
}

void Game::initWastePile() {
  wastePile_ = std::make_unique<WastePile>();
  connect(wastePile_.get(), &Pile::cardMoved, this, &Game::moveCard);
  connect(wastePile_.get(), &Pile::cardMoveAuto, this, &Game::moveCardAuto);
}

void Game::initKlondikePiles() {
  for (int i = 0; i < KLONDIKE_PILE_AM; i++) {
    if (deck_->Empty()) {
      throw std::out_of_range("pile empty");
    }
    klondikePiles_[i] = std::make_unique<KlondikePile>(*deck_, i + 1, 1);
    connect(klondikePiles_[i].get(), &Pile::cardMoved, this, &Game::moveCard);
    connect(klondikePiles_[i].get(), &Pile::cardMoveAuto, this,
            &Game::moveCardAuto);
  }
}

void Game::initTargetPiles() {
  for (size_t i = 0; i < TARGET_PILE_AM; ++i) {
    targetPiles_[i] = std::make_unique<TargetPile>(allSuits[i]);
    connect(targetPiles_[i].get(), &Pile::cardMoved, this, &Game::moveCard);
    connect(targetPiles_[i].get(), &Pile::cardMoveAuto, this,
            &Game::moveCardAuto);
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

bool Game::moveCard(Card* card, Pile* fromPile, Pile* toPile) {
  if (fromPile->Empty()) {
    return false;
  }

  int i = fromPile->cardIndexFromBack(card);
  qDebug() << i;

  if (toPile->IsValid(*card)) {
    fromPile->TransferCard(*toPile, i);
    fromPile->updateVisuals();
    toPile->updateVisuals();
    return true;
  }
  return false;
}

bool Game::moveCardAuto(Card* card, Pile* fromPile) {
  if (!hardMode) {
    if (fromPile->Empty()) {
      return false;
    }
    firstLegalMove(card, fromPile);
    return true;
  }
  return false;
}

void Game::firstLegalMove(Card* card, Pile* fromPile) {
  for (auto& ptr : targetPiles_) {
    Pile* toPile = ptr.get();
    if (toPile != fromPile && toPile->IsValid(*card)) {
      moveCard(card, fromPile, toPile);
      return;
    }
  }

  for (auto& ptr : klondikePiles_) {
    Pile* toPile = ptr.get();
    if (toPile != fromPile && toPile->IsValid(*card)) {
      moveCard(card, fromPile, toPile);
      return;
    }
  }
}
