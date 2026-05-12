#ifndef APPSETTINGSMANAGER_H
#define APPSETTINGSMANAGER_H

#include "AbstractSettingsManager.h"

class AppSettingsManager : public AbstractSettingsManager
{
    Q_OBJECT
    Q_PROPERTY(QString clientId READ clientId WRITE setClientId NOTIFY settingsChanged)
    Q_PROPERTY(QString clientSecret READ clientSecret WRITE setClientSecret NOTIFY settingsChanged)

public:
    explicit AppSettingsManager(QObject *parent = nullptr);

    QString clientId() const { return m_clientId; }
    QString clientSecret() const { return m_clientSecret; }

public slots:
    void setClientId(const QString &id);
    void setClientSecret(const QString &secret);

protected:
    void setParams(QSettings &settings) override;
    void getParams(QSettings &settings) override;
    void defaultSettings() override;

private:
    QString m_clientId;
    QString m_clientSecret;
};

#endif // APPSETTINGSMANAGER_H