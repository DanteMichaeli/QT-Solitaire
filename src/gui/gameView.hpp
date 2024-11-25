#include <QGraphicsScene>
#include <QGraphicsView>
#include <memory>
#include <vector>

#include "deck.hpp"
#include "klondikePile.hpp"

/**
 * @class GameView
 * @brief The main view for the Solitaire game, displaying the deck and a single
 * card.
 */

class GameView : public QGraphicsView {
 public:
  GameView(QWidget *parent = nullptr);

  void initializeGame();  // Initializes the scene and game components

 private:
  QGraphicsScene *scene;  // The scene containing all graphical items
  unique_ptr<Deck> deck_;
  std::vector<std::unique_ptr<KlondikePile>> klondikePiles;
};