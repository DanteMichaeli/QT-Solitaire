#include "game.hpp"

#include <exception>

Game::Game(QObject* parent)
    : klondikePiles_(KLONDIKE_PILE_AM),
      targetPiles_(TARGET_PILE_AM),
      points_(0),
      moves_(0),
      hints_(0),
      undos_(0),
      isWon_(false),
      prevHint_(nullptr),
      maxHistory_(0xFF),
      QObject(parent) {
  initDeck();
  initWastePile();
  initKlondikePiles();
  initTargetPiles();
  initTimer();
}

void Game::initDeck() {
  deck_ = new Deck();
  deck_->setParent(this);
  connect(deck_, &Pile::cardClickMove, this, &Game::handleDeckClicked);
}

void Game::initWastePile() {
  wastePile_ = new WastePile();
  wastePile_->setParent(this);
  connect(wastePile_, &Pile::cardMoved, this, &Game::handleMove);
  connect(wastePile_, &Pile::cardClickMove, this, &Game::handleAutoMove);
}

void Game::initKlondikePiles() {
  for (int i = 0; i < KLONDIKE_PILE_AM; i++) {
    if (deck_->isEmpty()) {
      throw std::out_of_range("pile empty");
    }
    KlondikePile* klondikePile = new KlondikePile();
    klondikePile->setParent(this);
    klondikePiles_[i] = klondikePile;
    connect(klondikePile, &Pile::cardMoved, this, &Game::handleMove);
    connect(klondikePile, &Pile::cardClickMove, this, &Game::handleAutoMove);
  }
}

void Game::initTargetPiles() {
  for (size_t i = 0; i < TARGET_PILE_AM; ++i) {
    TargetPile* targetPile = new TargetPile();
    targetPile->setParent(this);
    targetPiles_[i] = targetPile;
    connect(targetPile, &Pile::cardMoved, this, &Game::handleMove);
    connect(targetPile, &Pile::cardClickMove, this, &Game::handleAutoMove);
  }
}

void Game::initTimer() {
  elapsedTime_ = 0;
  timer_ = new QTimer(this);
  connect(timer_, &QTimer::timeout, this, &Game::elapseTime);
}

void Game::elapseTime() {
  elapsedTime_++;
  emit updateTime(elapsedTime_);
}

void Game::startGame() {
  deck_->setCardsPrevScenePos();
  for (size_t i = 0; i < klondikePiles_.size(); i++) {
    qDebug() << "Initializing KlondikePile with" << i << "cards.";
    KlondikePile* klondikePile = klondikePiles_[i];
    for (size_t j = 0; j <= i; j++) {
      deck_->transferCards(*klondikePile);
    }
    klondikePile->getTopCard()->flip();
    klondikePile->updateVisuals();
  }
  timer_->start(1000);
}

// Update stat.CSV when game is deconstructed and moves have been made
Game::~Game() {
  if (moves_ > 0 || undos_ > 0) {
    updateStats();
  }
}

void Game::logMove(const Move& move) {
  // Add move to history
  addToHistory(move);
  moves_++;
  points_ += move.pointChange_;

  // Determine if top card is flipped in KlondikePile.
  KlondikePile* kPile = dynamic_cast<KlondikePile*>(move.fromPile_);
  if (kPile && kPile->flipTopCard(true)) {
    addToHistory(
        Move(FLIP_KLONDIKE, move.fromPile_, move.toPile_, 0, turnOverPoints));
    points_ += turnOverPoints;
  }

  // Update visuals
  move.fromPile_->updateVisuals();
  move.toPile_->updateVisuals();
  prevHint_ = nullptr;

  // Update game view labels
  emit gameStateChange(points_, moves_);

  // Winning move?
  if (this->hasWon()) {
    isWon_ = true;
    soundManager_.playWinSound();
    emit gameWon(points_);
  }
}

void Game::updateStats() {
  GameStats stats = fromCSV("stats.csv");

  stats.games++;
  double games = static_cast<double>(stats.games);

  isWon_ ? stats.wins++ : stats.losses++;
  stats.winRate = stats.wins / games;

  stats.totalTime += elapsedTime_;
  if (isWon_) {
    if (stats.bestTime != 0) {
      stats.bestTime = std::min(stats.bestTime, elapsedTime_);
    } else {
      stats.bestTime = elapsedTime_;
    }
  }
  stats.avgTime = stats.totalTime / games;

  stats.totalMoves += moves_;
  stats.avgMoves = stats.totalMoves / games;
  if (isWon_) {
    if (stats.bestMoves != 0) {
      stats.bestMoves = std::min(stats.bestMoves, moves_);
    } else {
      stats.bestMoves = moves_;
    }
  }

  stats.hintCount += hints_;
  stats.undoCount += undos_;

  stats.totalPoints += points_;
  if (isWon_) stats.bestPoints = std::max(stats.bestPoints, points_);
  stats.avgPoints = stats.totalPoints / games;

  saveStatsToCSV("stats.csv", stats);
}

void Game::addToHistory(const Move& move) {
  // Keep the max size intact
  if (movehistory_.size() >= maxHistory_) {
    movehistory_.pop_front();
  }
  movehistory_.push_back(move);
}

void Game::undo() {
  if (!movehistory_.empty()) {
    moves_--;
    undos_++;
    prevHint_ = nullptr;

    // Check if card was flipped in KlondikePile
    if (movehistory_.back().type_ == FLIP_KLONDIKE) {
      const Move& flipMove = movehistory_.back();
      flipMove.fromPile_->flipTopCard(false);
      points_ -= flipMove.pointChange_;
      movehistory_.pop_back();
    }

    // Get last move
    const Move& move = movehistory_.back();
    movehistory_.pop_back();

    if (move.type_ == RECYCLE_DECK) {
      deck_->undoRecycle(*wastePile_);
    } else if (move.type_ == DECK_TO_WASTE) {
      wastePile_->undoAddFromDeck(*deck_, move.nofCards_);
    } else {
      move.toPile_->transferCards(*move.fromPile_, move.nofCards_);
    }

    // Update board
    points_ -= move.pointChange_;
    move.toPile_->updateVisuals();
    move.fromPile_->updateVisuals();
    emit gameStateChange(points_, moves_);
  }
}

int Game::pointChange(MoveType move) const {
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
  // Points cannot be negative, also prevent int underflow
  if (rawChange < 0 && points_ < -rawChange) {
    rawChange = -points_;
  }
  return rawChange;
}

void Game::changeSettings(const Settings& settings) {
  hardMode_ = settings.isHardModeEnabled;
  hintsEnabled_ = settings.isHintsEnabled;
  soundManager_.setVolume(settings.volume);
  qDebug() << "Updated settings:";
  qDebug() << "Volume:" << settings.volume;
  qDebug() << "Hints Enabled:" << settings.isHintsEnabled;
  qDebug() << "Hard Mode Enabled:" << settings.isHardModeEnabled;
}

int Game::attemptMove(Card* card, Pile* fromPile, Pile* toPile) {
  if (toPile != nullptr) {
    // How many cards are trying to be moved
    int i = fromPile->cardIndexFromTop(card);

    // Allow one card to move to a target pile
    if (i == 1 || dynamic_cast<TargetPile*>(toPile) == nullptr) {
      if (toPile->isValid(*card)) {
        fromPile->transferCards(*toPile, i);
        soundManager_.playMoveSound();
        return i;
      }
    }
  }
  return 0;
}

int Game::attemptDeckMove() {
  if (!deck_->isEmpty()) {
    int amount = hardMode_ ? 3 : 1;
    soundManager_.playMoveSound();
    return wastePile_->addFromDeck(*deck_, amount);
  } else if (deck_->recycle(*wastePile_)) {
    soundManager_.playShuffleSound();
    return -1;
  }
  return 0;
}

bool Game::hasWon() const {
  for (auto& ptr : targetPiles_) {
    if (ptr->getSize() != 13) {
      return false;
    }
  }
  return true;
}

MoveType Game::determineMove(Pile* fromPile, Pile* toPile) const {
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
    hints_ += 1;
    prevHint_ = findHint();
  }
  if (prevHint_ != nullptr) {
    prevHint_->animateGlow();
  }
}

Card* Game::findHint() const {
  // Klondike to Target / Klondike
  for (auto& klondikePile : klondikePiles_) {
    int i = 0;
    Card* currentCard = klondikePile->getCardFromBack(i);
    while (currentCard != nullptr && currentCard->isFaceUp()) {
      // Check Target Piles
      if (currentCard == klondikePile->getTopCard()) {
        for (auto& tp : targetPiles_) {
          if (tp->isValid(*currentCard)) {
            return currentCard;
          }
        }
      }
      // Check Klondike Piles
      Card* cardBelow = klondikePile->getCardFromBack(i + 1);
      if (currentCard->getRank() != KING || cardBelow != nullptr) {
        for (auto& kp : klondikePiles_) {
          if (kp != klondikePile && kp->isValid(*currentCard)) {
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
  // Waste to Target / Klondike
  Card* wasteTopCard = wastePile_->getTopCard();
  if (wasteTopCard != nullptr) {
    for (auto& tp : targetPiles_) {
      if (tp->isValid(*wasteTopCard)) {
        return wasteTopCard;
      }
    }
    for (auto& kp : klondikePiles_) {
      if (kp->isValid(*wasteTopCard)) {
        return wasteTopCard;
      }
    }
  }
  // Else give hint to deck
  if (!deck_->isEmpty()) {
    return deck_->getTopCard();
  }
  return nullptr;
}

void Game::handleAutoMove(Card* card, Pile* fromPile) {
  if (!hardMode_ && !fromPile->isEmpty()) {
    Pile* toPile = findLegalPile(card);
    int howMany = attemptMove(card, fromPile, toPile);
    if (howMany != 0) {
      const MoveType type = determineMove(fromPile, toPile);
      const Move move(determineMove(fromPile, toPile), fromPile, toPile,
                      howMany, pointChange(type));
      logMove(move);
    }
  }
}

void Game::handleMove(Card* card, Pile* fromPile,
                      const QPointF& scenePosition) {
  if (!fromPile->isEmpty()) {
    Pile* toPile = findPile(scenePosition);
    int howMany = attemptMove(card, fromPile, toPile);
    if (howMany != 0) {
      const MoveType type = determineMove(fromPile, toPile);
      const Move move(type, fromPile, toPile, howMany, pointChange(type));
      logMove(move);
    } else {
      fromPile->updateVisuals();  // Return to original position
    }
  }
}

void Game::handleDeckClicked() {
  int howMany = attemptDeckMove();
  if (howMany != 0) {
    if (howMany > 0) {  // -1 for recycle
      logMove(Move(DECK_TO_WASTE, deck_, wastePile_, howMany, dToWPoints));
    } else {
      logMove(
          Move(RECYCLE_DECK, wastePile_, deck_, 0, pointChange(RECYCLE_DECK)));
    }
  }
}

Pile* Game::findLegalPile(Card* card) const {
  Pile* parent = card->getPile();
  for (auto& pile : targetPiles_) {
    if (pile != parent && pile->isValid(*card) &&
        card == parent->getTopCard()) {  // One card to target pile at a time
      return pile;
    }
  }
  for (auto& pile : klondikePiles_) {
    if (pile != parent && pile->isValid(*card)) {
      return pile;
    }
  }
  return nullptr;
}

Pile* Game::findPile(const QPointF& scenePosition) const {
  for (auto& pile : targetPiles_) {
    if (pile->contains(pile->mapFromScene(scenePosition))) {
      return pile;
    }
  }
  for (auto& pile : klondikePiles_) {
    if (pile->contains(pile->mapFromScene(scenePosition))) {
      return pile;
    }
  }
  return nullptr;
}
