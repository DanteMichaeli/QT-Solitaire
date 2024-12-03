#ifndef GAMESOUNDMANAGER_HPP
#define GAMESOUNDMANAGER_HPP

#include <condition_variable>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

#include "../libs/miniaudio/miniaudio.h"

class GameSoundManager {
 public:
  GameSoundManager();
  ~GameSoundManager();

  void playSound(const std::string& filePath);  // Enqueue a sound for playback
  void stop();                                  // Stop the audio thread

 private:
  void audioThreadFunc();  // Thread function for processing sounds

  ma_engine engine_;                    // Audio engine
  std::queue<std::string> soundQueue_;  // Queue of sounds to play
  std::thread audioThread_;             // Background thread
  std::mutex queueMutex_;               // Mutex for thread safety
  std::condition_variable cv_;  // Condition variable for thread signaling
  bool stopThread_;             // Flag to stop the thread
};

#endif  // GAMESOUNDMANAGER_HPP