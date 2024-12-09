#ifndef GAME_VIEW_HPP
#define GAME_VIEW_HPP

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPushButton>
#include <QLabel>
#include <QResizeEvent>
#include <memory>
#include <vector>

#include "settings.hpp"
#include "game.hpp"
#include "layout.hpp"

/**
 * @class GameView
 * @brief The main view for the Solitaire game, displaying the deck and a single
 * card.
 */

class GameView : public QGraphicsView {
  Q_OBJECT
 public:
  GameView(QWidget *parent = nullptr, int volume = 100);

  void initializeGame();  // Initializes the scene and game components

  Layout &getLayout() { return *layout_; }

  void updateLayout(QSizeF &newSize);

  public slots:
  void settingsRelaySlot(Settings gameSettings); //get from mainwindow

  signals:
  void gameWon(int points);  // Relay signal to MainWindow.
  void settingsRelaySignal(Settings gameSettings); //Relay to game

 protected slots:
  void handleGameStateChange(int points, int moves);
  void handleGameWon(int points);

 private:
  QGraphicsScene *scene;  // The scene containing all graphical items
  unique_ptr<Game> game_;
  unique_ptr<Layout> layout_;
  QGraphicsProxyWidget *toolbarProxy_;
  QLabel *pointsLabel_;
  QLabel *moveLabel_;
  QPushButton *hintButton;
  void updateToolbarSize();
};
#endif
