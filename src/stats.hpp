#ifndef STATS_HPP
#define STATS_HPP

#include <string>

class Game;

/**
 * @brief Stat tracking functions for Solitaire game.
 *
 * This module contains functions and structures for tracking and managing game
 * statistics, such as wins, losses, average time, moves, undo count, and more.
 * It provides functionality to save and load stats from a CSV file.
 *
 * The `GameStats` structure holds various metrics, including the number of
 * games played, wins, losses, win rate, average time, and other relevant
 * statistics that track the player's performance.
 */
struct GameStats {
  unsigned int games;   ///< Total number of games played
  unsigned int wins;    ///< Total number of games won
  unsigned int losses;  ///< Total number of games lost
  double winRate;       ///< Win rate (wins / games)

  unsigned long totalTime;  ///< Total time spent playing all games
  unsigned int bestTime;    ///< Best time in a single game
  double avgTime;           ///< Average time spent per game

  unsigned long totalMoves;  ///< Total moves made across all games
  unsigned int bestMoves;    ///< Best moves in a single game
  double avgMoves;           ///< Average moves per game

  unsigned int undoCount;  ///< Total number of undo actions used
  unsigned int hintCount;  ///< Total number of hint requests made

  unsigned long totalPoints;  ///< Total points scored across all games
  unsigned int bestPoints;    ///< Highest points scored in a single game
  double avgPoints;           ///< Average points scored per game
};

/**
 * @brief Creates an initial stats file with default values.
 *
 * This function creates a CSV file for storing game statistics, initializing it
 * with default values.
 *
 * @param fileName The name of the file to create and store the statistics.
 */
void createInitialStatsFile(const std::string& fileName);

/**
 * @brief Saves the game statistics updates the CSV file.
 *
 * This function takes the current game statistics and saves them to the
 * specified CSV file.
 *
 * @param fileName The name of the CSV file to save the statistics.
 * @param stats The game statistics to save.
 */
void saveStatsToCSV(const std::string& fileName, const GameStats& stats);

/**
 * @brief Loads statistics from a CSV file.
 *
 * This function reads the statistics stored in a CSV file and returns a
 * `GameStats` object containing the loaded data.
 *
 * @param fileName The name of the CSV file to load statistics from.
 * @return The loaded game statistics.
 */
GameStats fromCSV(const std::string& fileName);

#endif  // STATS_HPP
