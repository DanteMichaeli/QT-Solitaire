#ifndef GAME_VIEW_HPP
#define GAME_VIEW_HPP

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLabel>
#include <QPushButton>
#include <QToolButton>
#include <memory>

#include "game.hpp"
#include "layout.hpp"
#include "settings.hpp"

enum DropDownOption { DD_NEW_GAME, DD_SETTINGS, DD_MAIN_MENU, DD_QUIT };

/**
 * @class GameView
 * @brief The main view for the Solitaire game, handling the display of the
 * deck, cards, toolbar, and interactions with the user.
 *
 * This class inherits from QGraphicsView and provides methods to initialize the
 * view, update the layout, and handle game state changes. It also includes
 * signals for notifying when the game is won or when a dropdown menu option is
 * selected.
 */
class GameView : public QGraphicsView {
  Q_OBJECT

 public:
  /**
   * @brief Constructs a GameView object.
   *
   * Initializes the view with the given settings and parent widget.
   *
   * @param settings The settings to be applied to the game and game view.
   * @param parent The parent widget for the view (default is nullptr).
   */
  GameView(Settings &settings, QWidget *parent = nullptr);

  /**
   * @brief Default destructor for the GameView object.
   */
  ~GameView() = default;

  /**
   * @brief Updates the layout of the view based on the new screen size.
   *
   * @param newSize The new screen size to update the layout to.
   */
  void updateLayout(const QSizeF &newSize);

  /**
   * @brief Starts a new game.
   *
   * starts the game timer and "deals" the cards.
   */
  void startGame() { game_->startGame(); }

  /**
   * @brief Changes the settings of the game and game view.
   *
   * @param gameSettings The new settings to apply to the game.
   */
  void changeSettings(const Settings &gameSettings);

 signals:
  /**
   * @brief Signal emitted when the game is won.
   *
   * @param points The points scored by the player.
   */
  void gameWon(const unsigned int points);

  /**
   * @brief Signal emitted when a dropdown option is selected.
   *
   * @param option The selected dropdown option.
   */
  void dropdownSignal(DropDownOption option);

 private slots:
  /**
   * @brief Slot to handle changes in the game state (points and moves).
   *
   * @param points The current score of the game.
   * @param moves The number of moves made so far.
   */
  void handleGameStateChange(const unsigned int points,
                             const unsigned int moves);

  /**
   * @brief Slot to handle the event when the game is won.
   *
   * @param points The points scored when the game is won.
   */
  void handleGameWon(const unsigned int points);

  /**
   * @brief Slot to handle the event when time has elapsed during the game.
   *
   * @param elapsedTime The elapsed time in seconds.
   */
  void handleTimeElapsed(const unsigned int elapsedTime);

 private:
  QGraphicsScene *scene_;       ///< The scene containing all graphical items
  std::unique_ptr<Game> game_;  ///< The game logic handling the Solitaire game
  std::unique_ptr<Layout>
      layout_;  ///< The layout managing the visual arrangement of items

  QWidget
      *toolbarWidget_;   ///< The toolbar widget containing buttons and labels
  QLabel *pointsLabel_;  ///< The label displaying the player's points
  QLabel *moveLabel_;    ///< The label displaying the number of moves made
  QLabel *timerLabel_;   ///< The label displaying the elapsed time

  QPushButton *hintButton_;  ///< Button to provide a hint to the player
  QPushButton *undoButton_;  ///< Button to undo the last move
  QToolButton *menuButton_;  ///< Button to open the dropdown menu

  /**
   * @brief Updates the size of the toolbar based on the window size.
   *
   * @param size The new size to update the toolbar to.
   */
  void updateToolbarSize(const QSizeF &size);

  /**
   * @brief Initializes the view by setting up the scene and other components.
   */
  void initView();

  /**
   * @brief Initializes the buttons (hint, undo, menu).
   */
  void initButtons();

  /**
   * @brief Initializes the labels (points, moves, timer).
   */
  void initLabels();

  /**
   * @brief Initializes the toolbar, including buttons and labels.
   */
  void initToolbar();
};

#endif  // GAME_VIEW_HPP
