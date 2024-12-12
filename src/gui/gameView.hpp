#ifndef GAME_VIEW_HPP
#define GAME_VIEW_HPP

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLabel>
#include <QPushButton>
#include <QResizeEvent>
#include <QToolButton>
#include <memory>
#include <vector>

#include "game.hpp"
#include "layout.hpp"
#include "settings.hpp"

enum DropDownOption { DD_NEW_GAME, DD_SETTINGS, DD_MAIN_MENU, DD_QUIT };

/**
 * @class GameView
 * @brief The main view for the Solitaire game, displaying the deck and a single
 * card.
 */
class GameView : public QGraphicsView {
  Q_OBJECT
 public:
  GameView(Settings &settings, QWidget *parent = nullptr);

  ~GameView() { qDebug() << "View destroyed"; }

  void initView();

  void initButtons();

  void initLabels();

  void initToolbar();

  Layout &getLayout() { return *layout_; }

  void updateLayout(QSizeF &newSize);

  void startGame() { game_->startGame(); }

  void changeSettings(Settings &gameSettings);

 signals:
  void gameWon(int points);  // Relay signal to MainWindow.
  void dropdownSignal(DropDownOption option);

 protected slots:
  void handleGameStateChange(int points, int moves);
  void handleGameWon(int points);
  void handleTimeElapsed(unsigned long elapsedTime);

 private:
  QGraphicsScene *scene_;  // The scene containing all graphical items
  unique_ptr<Game> game_;
  unique_ptr<Layout> layout_;

  QWidget *toolbarWidget_;
  QLabel *pointsLabel_;
  QLabel *moveLabel_;
  QLabel *timerLabel_;

  QPushButton *hintButton_;
  QPushButton *undoButton_;
  QToolButton *menuButton_;
  void updateToolbarSize(QSizeF &size);
};
#endif
