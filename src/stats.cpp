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
      file << "Wins,Losses,WinRate,AvgTime,AvgMoves,UndoCount,HintCount,"
              "LeaveRate,BestPoints,AvgPoints,Games\n";
      file << "0,0,0.0,0.0,0,0,0,0.0,0,0.0,0\n";

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
    file << "Wins,Losses,WinRate,AvgTime,AvgMoves,UndoCount,HintCount,"
            "LeaveRate,BestPoints,AvgPoints,games\n";

    // Write each record
    file << stats.wins << "," << stats.losses << "," << stats.winRate << ","
         << stats.avgTime << "," << stats.avgMoves << "," << stats.undoCount
         << "," << stats.hintCount << "," << stats.leaveRate << ","
         << stats.bestPoints << "," << stats.avgPoints << "," << stats.games
         << "\n";

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
      stats.wins = std::stoi(cell);
      std::getline(lineStream, cell, ',');
      stats.losses = std::stoi(cell);
      std::getline(lineStream, cell, ',');
      stats.winRate = std::stod(cell);
      std::getline(lineStream, cell, ',');
      stats.avgTime = std::stod(cell);
      std::getline(lineStream, cell, ',');
      stats.avgMoves = std::stod(cell);
      std::getline(lineStream, cell, ',');
      stats.undoCount = std::stoi(cell);
      std::getline(lineStream, cell, ',');
      stats.hintCount = std::stoi(cell);
      std::getline(lineStream, cell, ',');
      stats.leaveRate = std::stod(cell);
      std::getline(lineStream, cell, ',');
      stats.bestPoints = std::stoi(cell);
      std::getline(lineStream, cell, ',');
      stats.avgPoints = std::stod(cell);
      std::getline(lineStream, cell, ',');
      stats.games = std::stoi(cell);
    }

    file.close();
    std::cout << "Overall stats loaded from " << fileName << std::endl;
  } else {
    std::cerr << "Failed to open " << fileName << " for reading!" << std::endl;
  }

  return stats;
}
