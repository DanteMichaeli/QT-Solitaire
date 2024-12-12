#include "settings.hpp"

QJsonObject Settings::toJson() const {
  QJsonObject json;
  json["volume"] = volume;
  json["hints"] = isHintsEnabled;
  json["hardmode"] = isHardModeEnabled;
  return json;
}

// Function to load from a QJsonObject into the Settings struct
void Settings::fromJson(const QJsonObject& json) {
  if (json.contains("volume") && json["volume"].isDouble())
    volume = json["volume"].toInt();
  if (json.contains("hints") && json["hints"].isBool())
    isHintsEnabled = json["hints"].toBool();
  if (json.contains("hardmode") && json["hardmode"].isBool())
    isHardModeEnabled = json["hardmode"].toBool();
}

// Function to save settings to a JSON file
bool saveSettingsToJSON(const Settings& settings, const QString& filePath) {
  QJsonObject json = settings.toJson();
  QJsonDocument doc(json);
  QFile file(filePath);

  if (!file.open(QIODevice::WriteOnly)) {
    qWarning() << "Could not open file for writing:" << filePath;
    return false;
  }

  file.write(doc.toJson());
  file.close();
  return true;
}

// Function to load settings from a JSON file
bool loadSettingsFromJSON(Settings& settings, const QString& filePath) {
  QFile file(filePath);

  if (!file.open(QIODevice::ReadOnly)) {
    qWarning() << "Could not open file for reading:" << filePath;
    return false;
  }

  QByteArray data = file.readAll();
  file.close();

  QJsonDocument doc = QJsonDocument::fromJson(data);
  if (doc.isNull() || !doc.isObject()) {
    qWarning() << "Invalid JSON in file:" << filePath;
    return false;
  }

  QJsonObject json = doc.object();
  settings.fromJson(json);
  return true;
}