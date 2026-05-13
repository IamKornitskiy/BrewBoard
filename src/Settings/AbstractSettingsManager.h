#ifndef ABSTRACTSETTINGSMANAGER_H
#define ABSTRACTSETTINGSMANAGER_H

#include <QObject>
#include <QSettings>

class AbstractSettingsManager : public QObject
{
    Q_OBJECT
public:
    explicit AbstractSettingsManager(const QString& groupName, QObject* parent = nullptr);
    virtual ~AbstractSettingsManager() = default;

    void saveSettings(); // Запись файла настроек
    void loadSettings(); // Чтение файла настроек

protected:
    virtual void setParams(QSettings& settings) = 0; // Запись параметров
    virtual void getParams(QSettings& settings) = 0; // Чтение параметров
    virtual void defaultSettings() = 0; // Установка параметров по умолчанию

    QString m_GroupName; // Название группы настроек

signals:
    void settingsChanged();
};

#endif // ABSTRACTSETTINGSMANAGER_H