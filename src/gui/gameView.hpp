#ifndef GAME_VIEW_HPP
#define GAME_VIEW_HPP

#include <QGraphicsScene>
#include <QGraphicsView>
#include <memory>
#include <vector>

#include "game.hpp"

/**
 * @class GameView
 * @brief The main view for the Solitaire game, displaying the deck and a single
 * card.
 */

class GameView : public QGraphicsView {

Q_OBJECT

public:
  GameView(QWidget *parent = nullptr);

  void initializeGame();  // Initializes the scene and game components

  signals:
  void gameWon(int points);  // Relay signal to MainWindow.

  private slots:
  void handleGameWon(int points);

 private:

  QGraphicsScene *scene;  // The scene containing all graphical items
  unique_ptr<Game> game_;
};
#endif
