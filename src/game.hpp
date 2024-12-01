#ifndef GAME_HPP
#define GAME_HPP

#include <QObject>
#include <stack>

#include "deck.hpp"
#include "klondikePile.hpp"
#include "targetPile.hpp"
#include "wastePile.hpp"

#define KLONDIKE_PILE_AM 7
#define TARGET_PILE_AM 4

enum MoveType {
  WASTE_TO_KLONDIKE = 5,
  WASTE_TO_TARGET = 10,
  KLONDIKE_TO_TARGET = 10,
  KLONDIKE_TO_KLONDIKE = 0,
  TURN_OVER_KLONDIKE = 5,
  TARGET_TO_KLONDIKE = -15,
  DECK_TO_WASTE = 0,
  RECYCLE_DECK = -100,
  UNKNOWN = 0
};

struct Move {
  MoveType type_;
  Pile* fromPile_;  // Use of reference -> no need for dynamic memory control.
  Pile* toPile_;
  int nofCards_;
  Move(MoveType type, Pile* fromPile, Pile* toPile, int nofCards)
      : type_(type),
        fromPile_(fromPile),
        toPile_(toPile),
        nofCards_(nofCards) {}
};

using namespace std;
class Game : public QObject {
 public:
  Game(QObject* parent = nullptr);

  ~Game() {}

  void initDeck();
  void initWastePile();
  void initKlondikePiles();
  void initTargetPiles();

  Deck* getDeck() const { return deck_.get(); }

  WastePile* getWPile() const { return wastePile_.get(); }

  vector<unique_ptr<KlondikePile>>& getKPiles() { return klondikePiles_; }

  vector<unique_ptr<TargetPile>>& getTPiles() { return targetPiles_; }

  void toggleHardMode() { hardMode = !hardMode; }

  Pile* findLegalPile(Card* card);
  Pile* findPile(QPointF scenePosition);

  int attemptMove(Card* card, Pile* fromPile, Pile* toPile);
  int attemptDeckMove();
  void move(Move move);

  MoveType determineMove(Pile* fromPile, Pile* toPile);

  void updatePoints(MoveType move);

  bool hasWon();

 private slots:
  void handleDeckClicked();
  void handleMove(Card* card, Pile* fromPile, QPointF scenePosition);
  void handleAutoMove(Card* card, Pile* fromPile);

 private:
  unique_ptr<Deck> deck_;
  unique_ptr<WastePile> wastePile_;
  vector<unique_ptr<KlondikePile>> klondikePiles_;
  vector<unique_ptr<TargetPile>> targetPiles_;
  size_t points_;
  bool hardMode = false;
  bool isWon = false;
  stack<Move> movehistory_;
};

#endif