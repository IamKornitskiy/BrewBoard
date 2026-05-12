#include "AbstractSettingsManager.h"
#include "LoggerMacros.h"

AbstractSettingsManager::AbstractSettingsManager(const QString &groupName, QObject *parent)
    : QObject{parent}
    , m_GroupName(groupName)
{}

//Запись файла настроек
void AbstractSettingsManager::saveSettings()
{
    QSettings settings;

    settings.beginGroup(m_GroupName);
    setParams(settings); //Запись настроек
    settings.endGroup();
}

//Чтение файла настроек
void AbstractSettingsManager::loadSettings()
{
    QSettings settings;

    settings.beginGroup(m_GroupName);
    getParams(settings); //Чтение настроек
    settings.endGroup();
}