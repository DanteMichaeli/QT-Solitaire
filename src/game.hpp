#ifndef GAME_HPP
#define GAME_HPP

#include <QObject>

#include "deck.hpp"
#include "klondikePile.hpp"
#include "targetPile.hpp"
#include "wastePile.hpp"

#define KLONDIKE_PILE_AM 7
#define TARGET_PILE_AM 4

using namespace std;
class Game : public QObject {
 public:
  Game(QObject* parent = nullptr);

  ~Game() {}

  void initGame();

  Deck* getDeck() const { return deck_.get(); }

  WastePile* getWPile() const { return wastePile_.get(); }

  vector<unique_ptr<KlondikePile>>& getKPiles() { return klondikePiles_; }

  vector<unique_ptr<TargetPile>>& getTPiles() { return targetPiles_; }

 private slots:
  void deckClicked();

 private:
  unique_ptr<Deck> deck_;
  unique_ptr<WastePile> wastePile_;
  vector<unique_ptr<KlondikePile>> klondikePiles_;
  vector<unique_ptr<TargetPile>> targetPiles_;
};

#endif