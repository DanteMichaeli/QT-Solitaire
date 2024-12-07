#ifndef STATS_HPP
#define STATS_HPP
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "game.hpp"

/**
 * @brief Stat tracking functions
 * Assumes all data is in the form of Wins, Losses, Win rate, Average time,
 * Average moves, undo count, hint count, Leave Rate, Best & average points
 *
 */

struct GameStats {
  int wins;
  int losses;
  double winRate;
  double avgTime;
  int avgMoves;
  int undoCount;
  int hintCount;
  double leaveRate;
  int bestPoints;
  double avgPoints;
  int games;
};

void createInitialStatsFile(const std::string& fileName);
void saveStatsToCSV(const std::string& fileName, const GameStats& stats);
GameStats fromCSV(const std::string& fileName);

#endif