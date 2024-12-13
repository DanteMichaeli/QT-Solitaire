#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QResizeEvent>
#include <QStackedWidget>

#include "gameView.hpp"
#include "settings.hpp"

namespace Ui {

class MainWindow;
}

/**
 * @enum Window
 * @brief Enum representing different windows in the main window stack.
 *
 * The `Window` enum is used to track and switch between different views or
 * screens in the main window, such as the menu, settings, game, win screen, and
 * statistics.
 */
enum Window { MENU, SETTINGS, GAME, WIN_SCREEN, STATS };

/**
 * @class MainWindow
 * @brief Main window for managing the different screens in the Solitaire game.
 *
 * The `MainWindow` class extends `QMainWindow` and provides functionality for
 * switching between views, handling settings, starting/continuing a game, and
 * displaying statistics. It uses a `QStackedWidget` to manage multiple pages
 * (menu, game, win screen, etc.).
 */
class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  /**
   * @brief Constructs a MainWindow object.
   *
   * Initializes the main window and sets up necessary components for managing
   * different views in the game.
   *
   * @param parent The parent widget for this window (default is nullptr).
   */
  explicit MainWindow(QWidget *parent = nullptr);

  /**
   * @brief Destructor for the MainWindow object.
   *
   * Cleans up resources when the main window is closed.
   */
  ~MainWindow();

  /**
   * @brief Switches the window to fullscreen mode.
   */
  void fullscreen();

  /**
   * @brief Opens the settings window.
   */
  void openSettings();

  /**
   * @brief Quits the application.
   */
  void quit();

  /**
   * @brief Switches to the main menu.
   */
  void toMenu();

  /**
   * @brief Returns to the previous window.
   */
  void returnToPrev();

  /**
   * @brief Initializes the transition back to the main menu.
   */
  void backToMenuInit();

  /**
   * @brief Switches to the statistics window.
   */
  void toStatistics();

  /**
   * @brief Starts a new game.
   */
  void startGame();

  /**
   * @brief Continues an ongoing game.
   */
  void continueGame();

  /**
   * @brief Deletes the current game.
   */
  void deleteGame();

  /**
   * @brief Initializes a new game setup.
   */
  void initNewGame();

  /**
   * @brief Checks if a game has been initialized.
   *
   * @return `true` if the game is initialized, `false` otherwise.
   */
  bool gameInitialized() const;

  /**
   * @brief Checks if a game has started.
   *
   * @return `true` if the game has started, `false` otherwise.
   */
  bool gameStarted() const;

  /**
   * @brief Sets the game as started or not.
   *
   * @param value The value to set for the game start status (true or false).
   */
  void setGameStarted(bool value);

  /**
   * @brief Formats the elapsed time in seconds into a readable string.
   *
   * @param seconds The number of seconds to format.
   * @return A string representing the formatted time: h:mm:ss.
   */
  static QString formatTime(const unsigned long seconds);

 private:
  Ui::MainWindow *ui;  ///< Pointer to the UI object for the main window
  QStackedWidget *stackedWidget_;  ///< Widget container for managing different
                                   ///< pages (menu, game, etc.)
  GameView *gameView_;             ///< Pointer to the GameView page
  Settings gameSettings_;          ///< Struct object for managing game settings
  Window
      previousWindow_;  ///< Keeps track of the previous window for navigation
  bool gameStarted_;    ///< Flag to track whether a game is running

  /**
   * @brief Switches the main window to a specific page.
   *
   * This method changes the view to one of the predefined windows (e.g., menu,
   * game).
   *
   * @param window The window to switch to (MENU, SETTINGS, GAME, WIN_SCREEN, or
   * STATS).
   */
  void switchToPage(const Window window);

  /**
   * @brief Loads the saved settings for the game.
   */
  void loadSettings();

  /**
   * @brief Saves the current settings of the game.
   */
  void saveSettings();

  /**
   * @brief Handles the window resize event.
   *
   * This function is called when the main window is resized, allowing
   * adjustments to the layout.
   *
   * @param event The resize event containing information about the new size.
   */
  void resizeEvent(QResizeEvent *event) override;

 private slots:
  /**
   * @brief Slot to handle the event when the game is won.
   *
   * This slot is triggered when the player wins the game, allowing the game to
   * show the final score or message.
   *
   * @param points The number of points scored by the player.
   */
  void onGameWon(const unsigned int points);

  /**
   * @brief Slot to handle dropdown menu actions.
   *
   * This slot handles the selected option from the dropdown menu (e.g., new
   * game, settings).
   *
   * @param option The selected dropdown option (new game, settings, etc.).
   */
  void fromDropdownSlot(const DropDownOption option);
};

#endif  // MAINWINDOW_H
