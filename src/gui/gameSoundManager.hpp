#ifndef GAMESOUNDMANAGER_HPP
#define GAMESOUNDMANAGER_HPP

#include <condition_variable>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

#include "../libs/miniaudio/miniaudio.h"

/**
 * @class GameSoundManager
 * @brief Manages the playback of game sounds.
 *
 * The GameSoundManager class handles the playback of various sounds in a game,
 * such as move, win, and shuffle sounds. It manages sound playback using a
 * separate background thread and provides methods to enqueue sounds and control
 * volume.
 */
class GameSoundManager {
 public:
  /**
   * @brief Constructs a GameSoundManager instance.
   * Initializes the audio engine and starts the audio processing thread.
   */
  GameSoundManager();

  /**
   * @brief Destroys the GameSoundManager instance.
   * Stops the audio thread and releases resources.
   */
  ~GameSoundManager();

  /**
   * @brief Enqueues a sound for playback.
   *
   * Adds the specified sound file to the playback queue for processing.
   * @param filePath The file path of the sound to play.
   */
  void playSound(const std::string& filePath);

  /**
   * @brief Plays a sound associated with a move action.
   * Plays a predefined sound indicating a move.
   */
  void playMoveSound();

  /**
   * @brief Plays a sound associated with a win action.
   * Plays a predefined sound indicating a win.
   */
  void playWinSound();

  /**
   * @brief Plays a sound associated with shuffling the deck.
   * Plays a predefined sound indicating the deck has been shuffled.
   */
  void playShuffleSound();

  /**
   * @brief Stops the audio processing thread.
   * This function stops the background audio thread, preventing further sound
   * playback.
   */
  void stop();

  /**
   * @brief Sets the volume for sound playback.
   *
   * Adjusts the volume level for all sounds played through the manager.
   * @param volume The desired volume level (usually between 0 and 100).
   */
  void setVolume(int volume);

 private:
  /**
   * @brief The function executed by the audio thread.
   *
   * This function runs in the background to process and play sounds from the
   * queue. It waits for new sounds to be enqueued and plays them sequentially.
   */
  void audioThreadFunc();

  ma_engine engine_;                    ///< The audio engine used for playback.
  std::queue<std::string> soundQueue_;  ///< Queue holding sounds to be played.
  std::thread audioThread_;  ///< The background thread for audio playback.
  std::mutex queueMutex_;  ///< Mutex for ensuring thread safety when accessing
                           ///< the sound queue.
  std::condition_variable
      cv_;           ///< Condition variable for signaling the audio thread.
  bool stopThread_;  ///< Flag indicating whether the audio thread should stop.
};

#endif  // GAMESOUNDMANAGER_HPP
