#include "stats.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

void createInitialStatsFile(const std::string& fileName) {
  // Check if the file already exists
  if (!std::filesystem::exists(fileName)) {
    std::ofstream file(fileName);

    if (file.is_open()) {
      // Write the header and initial values
      file << "Games,Wins,Losses,WinRate,TotalTime,BestTime,AvgTime,TotalMoves,"
              "BestMoves,AvgMoves,UndoCount,HintCount,TotalPoints,BestPoints,"
              "AvgPoints\n";
      file << "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0\n ";

      file.close();
      std::cout << "Initial stats file created: " << fileName << std::endl;
    } else {
      std::cerr << "Failed to create the stats file: " << fileName << std::endl;
    }
  }
}

void saveStatsToCSV(const std::string& fileName, const GameStats& stats) {
  std::ofstream file(fileName);

  if (file.is_open()) {
    // Write header row
    file << "Games,Wins,Losses,WinRate,TotalTime,BestTime,AvgTime,TotalMoves,"
            "BestMoves,AvgMoves,UndoCount,HintCount,TotalPoints,BestPoints,"
            "AvgPoints\n";

    // Write each record
    file << stats.games << "," << stats.wins << "," << stats.losses << ","
         << stats.winRate << "," << stats.totalTime << "," << stats.bestTime
         << "," << stats.avgTime << "," << stats.totalMoves << ","
         << stats.bestMoves << "," << stats.avgMoves << "," << stats.undoCount
         << "," << stats.hintCount << "," << stats.totalPoints << ","
         << stats.bestPoints << "," << stats.avgPoints << "\n";

    file.close();
  } else {
    std::cerr << "Failed to open file for writing: " << fileName << std::endl;
  }
};

GameStats fromCSV(const std::string& fileName) {
  GameStats stats;
  std::ifstream file(fileName);

  if (file.is_open()) {
    std::string line;

    // Skip header row
    std::getline(file, line);

    // Read the data row
    if (std::getline(file, line)) {
      std::stringstream lineStream(line);
      std::string cell;

      std::getline(lineStream, cell, ',');
      stats.games = std::stoul(cell);
      std::getline(lineStream, cell, ',');
      stats.wins = std::stoul(cell);
      std::getline(lineStream, cell, ',');
      stats.losses = std::stoul(cell);
      std::getline(lineStream, cell, ',');
      stats.winRate = std::stod(cell);

      std::getline(lineStream, cell, ',');
      stats.totalTime = std::stod(cell);
      std::getline(lineStream, cell, ',');
      stats.bestTime = std::stoul(cell);
      std::getline(lineStream, cell, ',');
      stats.avgTime = std::stod(cell);

      std::getline(lineStream, cell, ',');
      stats.totalMoves = std::stoul(cell);
      std::getline(lineStream, cell, ',');
      stats.bestMoves = std::stoul(cell);
      std::getline(lineStream, cell, ',');
      stats.avgMoves = std::stod(cell);

      std::getline(lineStream, cell, ',');
      stats.undoCount = std::stoul(cell);
      std::getline(lineStream, cell, ',');
      stats.hintCount = std::stoul(cell);

      std::getline(lineStream, cell, ',');
      stats.totalPoints = std::stoul(cell);
      std::getline(lineStream, cell, ',');
      stats.bestPoints = std::stoul(cell);
      std::getline(lineStream, cell, ',');
      stats.avgPoints = std::stod(cell);
    }

    file.close();
    std::cout << "Overall stats loaded from " << fileName << std::endl;
  } else {
    std::cerr << "Failed to open " << fileName << " for reading!" << std::endl;
  }

  return stats;
}
