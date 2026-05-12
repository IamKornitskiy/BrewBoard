#include "AppSettingsManager.h"
#include "LoggerMacros.h"

AppSettingsManager::AppSettingsManager(QObject *parent)
    : AbstractSettingsManager("API", parent)
{
    defaultSettings();
}

void AppSettingsManager::setClientId(const QString &id)
{
    if (m_clientId != id) {
        m_clientId = id;
        emit settingsChanged();
        LOG_INFO("Client ID updated");
    }
}

void AppSettingsManager::setClientSecret(const QString &secret)
{
    if (m_clientSecret != secret) {
        m_clientSecret = secret;
        emit settingsChanged();
        LOG_INFO("Client Secret updated");
    }
}

void AppSettingsManager::setParams(QSettings &settings)
{
    settings.setValue("clientId", m_clientId);
    settings.setValue("clientSecret", m_clientSecret);
}

void AppSettingsManager::getParams(QSettings &settings)
{
    m_clientId = settings.value("clientId").toString();
    m_clientSecret = settings.value("clientSecret").toString();
}

void AppSettingsManager::defaultSettings()
{
    m_clientId = "";
    m_clientSecret = "";
}