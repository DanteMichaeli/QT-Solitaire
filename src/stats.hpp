#ifndef STATS_HPP
#define STATS_HPP
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class Game;

/**
 * @brief Stat tracking functions
 * Assumes all data is in the form of Wins, Losses, Win rate, Average time,
 * Average moves, undo count, hint count, Best time, Best & average points
 *
 */

struct GameStats {
  size_t games;
  size_t wins;
  size_t losses;
  double winRate;

  size_t totalTime;
  size_t bestTime;
  double avgTime;

  size_t totalMoves;
  size_t bestMoves;
  double avgMoves;

  size_t undoCount;
  size_t hintCount;

  size_t totalPoints;
  size_t bestPoints;
  double avgPoints;
};

void createInitialStatsFile(const std::string& fileName);
void saveStatsToCSV(const std::string& fileName, const GameStats& stats);
GameStats fromCSV(const std::string& fileName);

#endif
