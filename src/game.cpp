#include "game.hpp"

#include <exception>

Game::Game(QObject* parent)
    : klondikePiles_(KLONDIKE_PILE_AM),
      targetPiles_(TARGET_PILE_AM),
      points_(0),
      maxHistory_(100),
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
  timer.start();
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
    targetPiles_[i] = std::make_unique<TargetPile>();
    connect(targetPiles_[i].get(), &Pile::cardMoved, this, &Game::handleMove);
    connect(targetPiles_[i].get(), &Pile::cardClickMove, this,
            &Game::handleAutoMove);
  }
}

void Game::logMove(Move& move) {
  addToHistory(move);
  points_ += move.pointChange_;

  // Determine if top card is flipped in KlondikePile.
  KlondikePile* kPile = dynamic_cast<KlondikePile*>(move.fromPile_);
  if (kPile && kPile->flipCard(true)) {
    Move flipMove =
        Move(FLIP_KLONDIKE, move.fromPile_, move.toPile_, 0, turnOverPoints);
    addToHistory(flipMove);
    points_ += flipMove.pointChange_;
  }
  // Update visuals
  move.fromPile_->updateVisuals();
  move.toPile_->updateVisuals();
  prevHint_ = nullptr;
  prevHint_ = nullptr;

  if (hasWon()) {
    updateStats();
    cout << "You won!" << endl;

    emit gameWon(points_);
  }
}

void Game::updateStats() {
  GameStats stats = fromCSV("stats.csv");
  std::cout << stats.games << std::endl;

  stats.hintCount += hints_;
  stats.avgMoves = (movehistory_.size() + stats.avgMoves * stats.games) /
                   (stats.games + 1.0);
  stats.avgPoints =
      (points_ + stats.avgPoints * stats.games) / (stats.games + 1.0);
  stats.wins += 1;
  stats.avgTime = (timer.elapsed() / 100.0 + stats.avgTime * stats.games) /
                  (stats.games + 1.0);
  stats.games += 1;
  std::cout << stats.games << std::endl;

  saveStatsToCSV("stats.csv", stats);
}

void Game::addToHistory(Move& move) {
  if (movehistory_.size() >= maxHistory_) {
    movehistory_.pop_front();
  }
  movehistory_.push_back(move);
}

void Game::undo() {
  if (!movehistory_.empty()) {
    Move& move = movehistory_.back();
    movehistory_.pop_back();

    // If last move is card flip, do 2 undos in that sense.
    if (move.type_ == FLIP_KLONDIKE) {
      static_cast<KlondikePile*>(move.fromPile_)->flipCard(false);
      move = movehistory_.back();
      movehistory_.pop_back();
      points_ -= move.pointChange_;
    }

    if (move.type_ == RECYCLE_DECK) {
      static_cast<Deck*>(move.toPile_)->undoRecycle(*wastePile_);
    } else if (move.type_ == DECK_TO_WASTE) {
      static_cast<WastePile*>(move.toPile_)
          ->undoAddFromDeck(*deck_, move.nofCards_);
    } else {
      move.toPile_->TransferCard(*move.fromPile_, move.nofCards_);
    }
    // Update board
    points_ -= move.pointChange_;
    move.toPile_->updateVisuals();
    move.fromPile_->updateVisuals();
  }
}

int Game::pointChange(MoveType move) {
  int rawChange;
  switch (move) {
    case (WASTE_TO_KLONDIKE):
      rawChange = wToKPoints;
      break;
    case (WASTE_TO_TARGET):
      rawChange = wToTPoints;
      break;
    case (KLONDIKE_TO_TARGET):
      rawChange = kToTPoints;
      break;
    case (KLONDIKE_TO_KLONDIKE):
      rawChange = kToKPoints;
      break;
    case (FLIP_KLONDIKE):
      rawChange = turnOverPoints;
      break;
    case (TARGET_TO_KLONDIKE):
      rawChange = tToKPoints;
      break;
    case (DECK_TO_WASTE):
      rawChange = dToWPoints;
      break;
    case (RECYCLE_DECK):
      rawChange = recycleDeckPoints;
      break;
    default:
      rawChange = 0;
  }

  if (rawChange < 0 && points_ - rawChange < 0) {
    rawChange = -points_;
  }
  return rawChange;
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
  fromPile->updateVisuals();  // Return the card to original position;
  return 0;
}

int Game::attemptDeckMove() {
  if (!deck_->Empty()) {
    int amount = 1;  // TODO: dynamic amount
    return wastePile_->AddFromDeck(*deck_, amount);
  } else if (deck_->recycle(*wastePile_)) {
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
  } else if (dynamic_cast<Deck*>(fromPile) &&
             dynamic_cast<WastePile*>(toPile)) {
    return DECK_TO_WASTE;
  } else if (dynamic_cast<WastePile*>(fromPile) &&
             dynamic_cast<Deck*>(toPile)) {
    return RECYCLE_DECK;
  }
  return UNKNOWN;
}

void Game::hint() {
  if (prevHint_ == nullptr) {
    prevHint_ = findHint();
  }
  if (prevHint_ != nullptr) {
    hints_ += 1;
    prevHint_->startGlowing();
  }
}

Card* Game::findHint() {
  for (auto& klondikePile : klondikePiles_) {
    int i = 0;
    Card* currentCard = klondikePile->getCardFromBack(i);
    while (currentCard != nullptr && currentCard->isFaceUp()) {
      for (auto& tp : targetPiles_) {
        if (tp->IsValid(*currentCard)) {
          return currentCard;
        }
      }
      if (currentCard->GetRank() != KING) {
        for (auto& kp : klondikePiles_) {
          if (kp != klondikePile && kp->IsValid(*currentCard)) {
            Card* cardBelow = klondikePile->getCardFromBack(i + 1);
            if (cardBelow == nullptr || !cardBelow->isFaceUp()) {
              return currentCard;
            }
          }
        }
      }
      i++;
      currentCard = klondikePile->getCardFromBack(i);
    }
  }
  Card* wasteTopCard = wastePile_->getTopCard();
  if (wasteTopCard != nullptr) {
    for (auto& tp : targetPiles_) {
      if (tp->IsValid(*wasteTopCard)) {
        return wasteTopCard;
      }
    }
    for (auto& kp : klondikePiles_) {
      if (kp->IsValid(*wasteTopCard)) {
        return wasteTopCard;
      }
    }
  }
  if (!deck_->Empty()) {
    return deck_->getTopCard();
  }
  return nullptr;
}

void Game::handleAutoMove(Card* card, Pile* fromPile) {
  if (!hardMode && !fromPile->Empty()) {
    Pile* toPile = findLegalPile(card);
    int howMany = attemptMove(card, fromPile, toPile);
    if (howMany != 0) {
      MoveType type = determineMove(fromPile, toPile);
      Move move = Move(type, fromPile, toPile, howMany, pointChange(type));
      logMove(move);
    }
  }
}

void Game::handleMove(Card* card, Pile* fromPile, QPointF scenePosition) {
  if (!fromPile->Empty()) {
    Pile* toPile = findPile(scenePosition);
    int howMany = attemptMove(card, fromPile, toPile);
    if (howMany != 0) {
      MoveType type = determineMove(fromPile, toPile);
      Move move = Move(type, fromPile, toPile, howMany, pointChange(type));
      logMove(move);
    } else {
      fromPile->updateVisuals();  // Return to original position
    }
  }
}

void Game::handleDeckClicked() {
  int howMany = attemptDeckMove();
  if (howMany != 0) {
    if (howMany > 0) {
      Move move = Move(DECK_TO_WASTE, deck_.get(), wastePile_.get(), howMany,
                       dToWPoints);
      logMove(move);
    } else {
      Move move = Move(RECYCLE_DECK, wastePile_.get(), deck_.get(), 0,
                       recycleDeckPoints);
      logMove(move);
    }
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
