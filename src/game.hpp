#ifndef GAME_HPP
#define GAME_HPP

#include <QObject>
#include <QTimer>
#include <deque>

#include "deck.hpp"
#include "gui/gameSoundManager.hpp"
#include "klondikePile.hpp"
#include "settings.hpp"
#include "stats.hpp"
#include "targetPile.hpp"
#include "wastePile.hpp"

#define KLONDIKE_PILE_AM 7  ///< Number of Klondike piles in the game.
#define TARGET_PILE_AM 4    ///< Number of target piles in the game.

/**
 * @brief Enum representing different types of moves in the game.
 *
 * Each move type has an associated point value that impacts the player's score.
 */
enum MoveType {
  WASTE_TO_KLONDIKE,
  WASTE_TO_TARGET,
  KLONDIKE_TO_TARGET,
  KLONDIKE_TO_KLONDIKE,
  FLIP_KLONDIKE,
  TARGET_TO_KLONDIKE,
  DECK_TO_WASTE,
  RECYCLE_DECK,
  UNKNOWN
};

/**
 * @brief Struct representing a move in the game.
 */
struct Move {
  const MoveType type_;  ///< The type of the move.
  Pile* fromPile_;      ///< Pointer to the pile from which the move originates.
  Pile* toPile_;        ///< Pointer to the pile to which the move is made.
  const int nofCards_;  ///< Number of cards involved in the move.
  const int pointChange_;

  Move(MoveType type, Pile* fromPile, Pile* toPile, int nofCards,
       int pointChange)
      : type_(type),
        fromPile_(fromPile),
        toPile_(toPile),
        nofCards_(nofCards),
        pointChange_(pointChange) {}
};

using namespace std;

/**
 * @brief The Game class manages the logic and state of the Klondike solitaire
 * game.
 */
class Game : public QObject {
  Q_OBJECT
 public:
  // Start table of move points.
  static const int wToKPoints = 5;
  static const int wToTPoints = 10;
  static const int kToTPoints = 10;
  static const int kToKPoints = 0;

  static const int turnOverPoints = 5;
  static const int tToKPoints = -15;
  static const int dToWPoints = 0;
  static const int recycleDeckPoints = -100;
  // End table of move Points.

  /**
   * @brief Constructs a Game object.
   *
   * @param parent Pointer to the parent QObject (default is nullptr).
   */
  Game(QObject* parent = nullptr);

  /**
   * @brief Destroys the Game object.
   */
  ~Game();

  /**
   * @brief Initializes the deck.
   */
  void initDeck();

  /**
   * @brief Initializes the waste pile.
   */
  void initWastePile();

  /**
   * @brief Initializes the Klondike piles.
   */
  void initKlondikePiles();

  /**
   * @brief Initializes the target piles.
   */
  void initTargetPiles();

  void initTimer();

  void startGame();

  void elapseTime();

  /**
   * @brief Retrieves the deck.
   * @return A pointer to the deck.
   */
  Deck* getDeck() const { return deck_; }

  /**
   * @brief Retrieves the waste pile.
   * @return A pointer to the waste pile.
   */
  WastePile* getWPile() const { return wastePile_; }

  /**
   * @brief Retrieves the Klondike piles.
   * @return A vector of unique pointers to Klondike piles.
   */
  const vector<KlondikePile*>& getKPiles() const { return klondikePiles_; }

  /**
   * @brief Retrieves the target piles.
   * @return A vector of unique pointers to target piles.
   */
  const vector<TargetPile*>& getTPiles() const { return targetPiles_; }

  /**
   * @brief Get player points.
   * @return points.
   */
  int getPoints() const { return points_; }

  int pointChange(MoveType move) const;

  void changeSettings(const Settings& settings);

  /**
   * @brief Toggles hard mode for the game.
   */
  void toggleHardMode() { hardMode_ = !hardMode_; }

  /**
   * @brief Finds the first legal pile for a card to be moved to.
   * @param card Pointer to the card being moved.
   * @return Pointer to the legal pile if found, otherwise nullptr.
   */
  Pile* findLegalPile(Card* card) const;

  /**
   * @brief Finds the pile at a given scene position.
   * @param scenePosition The position in the scene.
   * @return Pointer to the pile at the position if found, otherwise nullptr.
   */
  Pile* findPile(const QPointF& scenePosition) const;

  /**
   * @brief Attempts to move a card between piles.
   *
   * @param card Pointer to the card being moved.
   * @param fromPile Pointer to the originating pile.
   * @param toPile Pointer to the destination pile.
   * @return The point value of how many cards were moved.
   */
  int attemptMove(Card* card, Pile* fromPile, Pile* toPile);

  /**
   * @brief Attempts to move cards from the deck to the waste pile.
   * @return The point value of how many cards were moved.
   */
  int attemptDeckMove();

  /**
   * @brief Logs the move to history.
   * @param move The move that was executed.
   */
  void logMove(const Move& move);

  void addToHistory(const Move& move);

  /**
   * @brief undos the moves in history.
   */
  void undo();

  /**
   * @brief Determines the type of move between two piles.
   *
   * @param fromPile Pointer to the originating pile.
   * @param toPile Pointer to the destination pile.
   * @return The determined MoveType.
   */
  MoveType determineMove(Pile* fromPile, Pile* toPile) const;

  /**
   * @brief Checks if the player has won the game.
   * @return True if the player has won, otherwise false.
   */
  bool hasWon() const;

  void hint();

  Card* findHint() const;

  /**
   * @brief Updates game stats into file
   */
  void updateStats();

 signals:
  void updateTime(const unsigned int elapsedTime);
  void gameStateChange(const unsigned int points, const unsigned int moves);
  void gameWon(const unsigned int _t1);

 private slots:

  /**
   * @brief Handles the event when the deck is clicked.
   */
  void handleDeckClicked();

  /**
   * @brief Recieves a signal when card is moved and attempts to move cards
   * between piles.
   *
   * @param card Pointer to the card being moved.
   * @param fromPile Pointer to the originating pile.
   * @param scenePosition The position in the scene where the move ends.
   */
  void handleMove(Card* card, Pile* fromPile, const QPointF& scenePosition);

  /**
   * @brief Recieves a signal when card is clicked and attempts to move cards
   * between piles.
   * @param card Pointer to the card being moved.
   * @param fromPile Pointer to the originating pile.
   */
  void handleAutoMove(Card* card, Pile* fromPile);

 private:
  Deck* deck_;                           ///< The deck of cards.
  WastePile* wastePile_;                 ///< The waste pile.
  vector<KlondikePile*> klondikePiles_;  ///< The Klondike piles.
  vector<TargetPile*> targetPiles_;      ///< The target piles.

  QTimer* timer_;
  unsigned int elapsedTime_;

  unsigned int points_;  ///< The player's current score.
  unsigned int moves_;
  unsigned int hints_;
  unsigned int undos_;

  bool hardMode_;  ///< Indicates if the game is in hard mode.
  bool hintsEnabled_;
  bool isWon_;  ///< Indicates if the game has been won.
  unsigned int
      maxHistory_;  ///< The maximium amount of moves stored in the history.

  deque<Move> movehistory_;        ///< Stack storing the history of moves.
  GameSoundManager soundManager_;  ///< Game sound manager.
  Card* prevHint_;
};

#endif
