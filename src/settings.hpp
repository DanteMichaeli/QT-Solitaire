#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <QJsonDocument>
#include <QJsonObject>
#include <QString>

/**
 * @struct Settings
 * @brief Structure for holding game settings.
 *
 * The `Settings` structure stores various settings related to the game, such as
 * the volume, whether hints are enabled, and whether hard mode is active. It
 * provides methods for converting the settings to and from a JSON format for
 * easy saving and loading.
 */
struct Settings {
  int volume;              ///< The volume level for the game (0-100)
  bool isHintsEnabled;     ///< Whether hints are enabled (true if enabled)
  bool isHardModeEnabled;  ///< Whether hard mode is enabled (true if enabled)

  /**
   * @brief Converts the settings to a JSON object.
   *
   * This method serializes the `Settings` object into a `QJsonObject` that can
   * be saved to a JSON file.
   *
   * @return A `QJsonObject` representing the settings.
   */
  QJsonObject toJson() const;

  /**
   * @brief Loads settings from a JSON object.
   *
   * This method deserializes a `QJsonObject` and populates the `Settings`
   * object with the corresponding values.
   *
   * @param json The `QJsonObject` containing the settings to load.
   */
  void fromJson(const QJsonObject& json);
};

/**
 * @brief Saves the settings to a JSON file.
 *
 * This function serializes the `Settings` object and saves it to the specified
 * file path in JSON format.
 *
 * @param settings The `Settings` object to save.
 * @param filePath The file path where the settings should be saved.
 * @return `true` if the settings were successfully saved, `false` otherwise.
 */
bool saveSettingsToJSON(const Settings& settings, const QString& filePath);

/**
 * @brief Loads settings from a JSON file.
 *
 * This function reads a JSON file and loads the settings into the provided
 * `Settings` object.
 *
 * @param settings The `Settings` object where the loaded settings will be
 * stored.
 * @param filePath The file path from which to load the settings.
 * @return `true` if the settings were successfully loaded, `false` otherwise.
 */
bool loadSettingsFromJSON(Settings& settings, const QString& filePath);

#endif  // SETTINGS_HPP
