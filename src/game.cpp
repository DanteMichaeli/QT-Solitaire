#include "game.hpp"

#include <exception>

Game::Game(QObject* parent)
    : klondikePiles_(KLONDIKE_PILE_AM),
      targetPiles_(TARGET_PILE_AM),
      points_(0),
      QObject(parent) {
  initDeck();
  initWastePile();
  initKlondikePiles();
  initTargetPiles();
}

void Game::initDeck() {
  deck_ = std::make_unique<Deck>();
  deck_->Shuffle();
  connect(deck_.get(), &Pile::cardClickMove, this, &Game::handleDeckClicked);
}

void Game::initWastePile() {
  wastePile_ = std::make_unique<WastePile>();
  connect(wastePile_.get(), &Pile::cardMoved, this, &Game::handleAutoMove);
  connect(wastePile_.get(), &Pile::cardClickMove, this, &Game::handleAutoMove);
}

void Game::initKlondikePiles() {
  for (int i = 0; i < KLONDIKE_PILE_AM; i++) {
    if (deck_->Empty()) {
      throw std::out_of_range("pile empty");
    }
    klondikePiles_[i] = std::make_unique<KlondikePile>(*deck_, i + 1, 1);
    connect(klondikePiles_[i].get(), &Pile::cardMoved, this, &Game::handleMove);
    connect(klondikePiles_[i].get(), &Pile::cardClickMove, this,
            &Game::handleAutoMove);
  }
}

void Game::initTargetPiles() {
  for (size_t i = 0; i < TARGET_PILE_AM; ++i) {
    targetPiles_[i] = std::make_unique<TargetPile>(allSuits[i]);
    connect(targetPiles_[i].get(), &Pile::cardMoved, this, &Game::handleMove);
    connect(targetPiles_[i].get(), &Pile::cardClickMove, this,
            &Game::handleAutoMove);
  }
}

void Game::move(Move move) {
  movehistory_.push(move);
  updatePoints(move.type_);
  KlondikePile* kPile = dynamic_cast<KlondikePile*>(move.fromPile_);
  if (kPile != nullptr) {
    if (kPile->flipTopCard(true)) {
      movehistory_.push(Move(TURN_OVER_KLONDIKE, kPile, nullptr, 0));
      updatePoints(TURN_OVER_KLONDIKE);
    }
  }
  move.fromPile_->updateVisuals();
  move.toPile_->updateVisuals();
  qDebug() << points_;
  if (hasWon()) {
    cout << "You won!" << endl;
  }
}

void Game::updatePoints(MoveType move) {
  if ((points_ - move) < 0) {
    points_ = 0;
  } else {
    points_ += move;
  }
}

int Game::attemptMove(Card* card, Pile* fromPile, Pile* toPile) {
  if (toPile != nullptr) {
    int i = fromPile->cardIndexFromBack(card);
    if (i == 1 || dynamic_cast<TargetPile*>(toPile) == nullptr) {
      if (toPile->IsValid(*card)) {
        fromPile->TransferCard(*toPile, i);
        soundManager_.playSound(
            "src/assets/sounds/"
            "move.wav");
        fromPile->updateVisuals();
        toPile->updateVisuals();
        return i;
      }
    }
  }
  card->returnToPrevPos();  // Return the card to original position;
  return 0;
}

int Game::attemptDeckMove() {
  if (!deck_->Empty()) {
    int amount = 1;  // TODO: dynamic amount
    return wastePile_->AddFromDeck(*deck_, amount);
  } else if (deck_->recycle(*wastePile_, false)) {
    return -1;
  }
  return 0;
}

bool Game::hasWon() {
  for (auto& ptr : targetPiles_) {
    if (ptr->Size() != 13) {
      return false;
    }
  }
  return true;
}

MoveType Game::determineMove(Pile* fromPile, Pile* toPile) {
  if (dynamic_cast<WastePile*>(fromPile) &&
      dynamic_cast<KlondikePile*>(toPile)) {
    return WASTE_TO_KLONDIKE;
  } else if (dynamic_cast<WastePile*>(fromPile) &&
             dynamic_cast<TargetPile*>(toPile)) {
    return WASTE_TO_TARGET;
  } else if (dynamic_cast<KlondikePile*>(fromPile) &&
             dynamic_cast<TargetPile*>(toPile)) {
    return KLONDIKE_TO_TARGET;
  } else if (dynamic_cast<KlondikePile*>(fromPile) &&
             dynamic_cast<KlondikePile*>(toPile)) {
    return KLONDIKE_TO_KLONDIKE;
  } else if (dynamic_cast<TargetPile*>(fromPile) &&
             dynamic_cast<KlondikePile*>(toPile)) {
    return TARGET_TO_KLONDIKE;
  }
  return UNKNOWN;
}

void Game::handleAutoMove(Card* card, Pile* fromPile) {
  if (!hardMode && !fromPile->Empty()) {
    Pile* toPile = findLegalPile(card);
    int howMany = attemptMove(card, fromPile, toPile);
    if (howMany != 0) {
      MoveType type = determineMove(fromPile, toPile);
      Move thisMove = Move(type, fromPile, toPile, howMany);
      move(thisMove);
    }
  }
}

void Game::handleMove(Card* card, Pile* fromPile, QPointF scenePosition) {
  if (!fromPile->Empty()) {
    Pile* toPile = findPile(scenePosition);
    int howMany = attemptMove(card, fromPile, toPile);
    if (howMany != 0) {
      MoveType type = determineMove(fromPile, toPile);
      Move thisMove = Move(type, fromPile, toPile, howMany);
      move(thisMove);
    }
  }
}

void Game::handleDeckClicked() {
  qDebug() << "works";
  int res = attemptDeckMove();
  if (res != 0) {
    Move thisMove =
        (res > 0) ? Move(DECK_TO_WASTE, deck_.get(), wastePile_.get(), res)
                  : Move(RECYCLE_DECK, wastePile_.get(), deck_.get(), 0);
    move(thisMove);
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