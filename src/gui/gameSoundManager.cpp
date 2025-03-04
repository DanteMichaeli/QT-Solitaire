#define MINIAUDIO_IMPLEMENTATION
#include "gameSoundManager.hpp"

#include <filesystem>
#include <iostream>

#include "../libs/miniaudio/miniaudio.h"  // Adjust path if necessary

// Constructor
GameSoundManager::GameSoundManager() : stopThread_(false) {
  if (ma_engine_init(NULL, &engine_) != MA_SUCCESS) {
    std::cerr << "Failed to initialize audio engine." << std::endl;
    exit(-1);
  }

  // Start the audio processing thread
  audioThread_ = std::thread(&GameSoundManager::audioThreadFunc, this);
}

// Destructor
GameSoundManager::~GameSoundManager() {
  stop();  // Ensure the thread stops
  ma_engine_uninit(&engine_);
}

// Play a sound (enqueue the file path)
void GameSoundManager::playSound(const std::string& filePath) {
  {
    std::lock_guard<std::mutex> lock(queueMutex_);

    soundQueue_.push(filePath);
  }
  cv_.notify_one();  // Notify the audio thread
}

// Play a move sound
void GameSoundManager::playMoveSound() {
  playSound(
      "src/assets/sounds/"
      "move.wav");
}

// Play a win sound
void GameSoundManager::playWinSound() {
  playSound(
      "src/assets/sounds/"
      "win.wav");
}

// Play a shuffle deck sound
void GameSoundManager::playShuffleSound() {
  playSound(
      "src/assets/sounds/"
      "shuffle.wav");
}

// Stop the audio thread
void GameSoundManager::stop() {
  {
    std::lock_guard<std::mutex> lock(queueMutex_);
    stopThread_ = true;
  }
  cv_.notify_one();  // Notify the audio thread to exit
  if (audioThread_.joinable()) {
    audioThread_.join();
  }
}

void GameSoundManager::setVolume(int volume) {
  ma_engine_set_volume(&engine_, volume / 100.0);
}

// Thread function for processing sounds
void GameSoundManager::audioThreadFunc() {
  while (true) {
    std::string filePath;

    // Wait for a sound to be enqueued or for the thread to be stopped
    {
      std::unique_lock<std::mutex> lock(queueMutex_);
      cv_.wait(lock, [this] { return !soundQueue_.empty() || stopThread_; });

      if (stopThread_ && soundQueue_.empty()) {
        break;  // Exit the thread
      }

      filePath = soundQueue_.front();
      soundQueue_.pop();
    }

    // Resolve relative path to absolute path
    std::string resolvedPath = std::filesystem::absolute(filePath).string();

    // Play the sound
    if (ma_engine_play_sound(&engine_, resolvedPath.c_str(), NULL) !=
        MA_SUCCESS) {
      std::cerr << "Failed to play sound: " << resolvedPath << std::endl;
    } else {
      std::cout << "Playing sound: " << resolvedPath << std::endl;
    }
  }
}
