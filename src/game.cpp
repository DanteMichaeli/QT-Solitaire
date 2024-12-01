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
  connect(wastePile_.get(), &Pile::cardMoved, this, &Game::handleAutoMove);
  connect(wastePile_.get(), &Pile::cardMoveAuto, this, &Game::handleAutoMove);
}

void Game::initKlondikePiles() {
  for (int i = 0; i < KLONDIKE_PILE_AM; i++) {
    if (deck_->Empty()) {
      throw std::out_of_range("pile empty");
    }
    klondikePiles_[i] = std::make_unique<KlondikePile>(*deck_, i + 1, 1);
    connect(klondikePiles_[i].get(), &Pile::cardMoved, this, &Game::handleMove);
    connect(klondikePiles_[i].get(), &Pile::cardMoveAuto, this,
            &Game::handleAutoMove);
  }
}

void Game::initTargetPiles() {
  for (size_t i = 0; i < TARGET_PILE_AM; ++i) {
    targetPiles_[i] = std::make_unique<TargetPile>(allSuits[i]);
    connect(targetPiles_[i].get(), &Pile::cardMoved, this, &Game::handleMove);
    connect(targetPiles_[i].get(), &Pile::cardMoveAuto, this,
            &Game::handleAutoMove);
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

void Game::move(Card* card, Pile* fromPile, Pile* toPile) {
  if (attemptMove(card, fromPile, toPile)) {
    if (hasWon()) {
      cout << "You won!" << endl;
    }
  }
}

bool Game::attemptMove(Card* card, Pile* fromPile, Pile* toPile) {
  if (toPile != nullptr) {
    int i = fromPile->cardIndexFromBack(card);
    if (i == 1 || dynamic_cast<TargetPile*>(toPile) == nullptr) {
      if (toPile->IsValid(*card)) {
        fromPile->TransferCard(*toPile, i);
        fromPile->updateVisuals();
        toPile->updateVisuals();
        return true;
      }
    }
  }
  card->returnToPrevPos();  // Return the card to original position;
  return false;
}

bool Game::hasWon() {
  for (auto& ptr : targetPiles_) {
    if (ptr->Size() != 13) {
      return false;
    }
  }
  return true;
}

void Game::handleAutoMove(Card* card, Pile* fromPile) {
  if (!hardMode && !fromPile->Empty()) {
    move(card, fromPile, findLegalPile(card));
  }
}

void Game::handleMove(Card* card, Pile* fromPile, QPointF scenePosition) {
  if (!fromPile->Empty()) {
    move(card, fromPile, findPile(scenePosition));
  }
}

Pile* Game::findLegalPile(Card* card) {
  for (auto& ptr : targetPiles_) {
    Pile* pile = ptr.get();
    if (pile != card->parentItem() && pile->IsValid(*card)) {
      return pile;
    }
  }
  for (auto& ptr : klondikePiles_) {
    Pile* pile = ptr.get();
    if (pile != card->parentItem() && pile->IsValid(*card)) {
      return pile;
    }
  }
  return nullptr;
}

Pile* Game::findPile(QPointF scenePosition) {
  qDebug() << scenePosition;
  for (auto& ptr : targetPiles_) {
    Pile* pile = ptr.get();
    if (pile->contains(pile->mapFromScene(scenePosition))) {
      return pile;
    }
  }
  for (auto& ptr : klondikePiles_) {
    Pile* pile = ptr.get();
    if (pile->contains(pile->mapFromScene(scenePosition))) {
      return pile;
    }
  }
  return nullptr;
}