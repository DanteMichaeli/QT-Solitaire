#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QVariant>

struct Settings {
  int volume;
  bool isHintsEnabled;
  bool isHardModeEnabled;

  QJsonObject toJson() const;
  void fromJson(const QJsonObject& json);
};

bool saveSettingsToJSON(const Settings& settings, const QString& filePath);

bool loadSettingsFromJSON(Settings& settings, const QString& filePath);

#endif  // SETTINGS_HPP
