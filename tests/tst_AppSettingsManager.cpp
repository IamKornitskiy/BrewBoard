#include <QSettings>
#include <QtTest>

#include "AppSettingsManager.h"

class AppSettingsManagerSpy : public QObject
{
    Q_OBJECT
public:
    explicit AppSettingsManagerSpy(AppSettingsManager* target)
        : QObject(target)
    {
        connect(target, &AppSettingsManager::settingsChanged, this,
                &AppSettingsManagerSpy::onSettingsChanged);
        m_count = 0;
    }

    int count() const { return m_count; }

private slots:
    void onSettingsChanged() { ++m_count; }

private:
    int m_count;
};

class TestAppSettingsManager : public QObject
{
    Q_OBJECT

private:
    AppSettingsManager* createManager(const QString& fileName)
    {
        QSettings::setPath(QSettings::NativeFormat, QSettings::UserScope, QDir::tempPath());
        QSettings settings(fileName, QSettings::NativeFormat);
        settings.clear();

        AppSettingsManager* manager = new AppSettingsManager();
        return manager;
    }

private slots:
    void testDefaultValues()
    {
        AppSettingsManager manager;
        QCOMPARE(manager.clientId(), QString(""));
        QCOMPARE(manager.clientSecret(), QString(""));
    }

    void testSetClientId()
    {
        AppSettingsManager manager;
        QSignalSpy spy(&manager, &AppSettingsManager::settingsChanged);

        manager.setClientId("test-id");
        QCOMPARE(manager.clientId(), QString("test-id"));
        QCOMPARE(spy.count(), 1);

        spy.clear();
        manager.setClientId("test-id");
        QCOMPARE(manager.clientId(), QString("test-id"));
        QCOMPARE(spy.count(), 0);
    }

    void testSetClientSecret()
    {
        AppSettingsManager manager;
        QSignalSpy spy(&manager, &AppSettingsManager::settingsChanged);

        manager.setClientSecret("test-secret");
        QCOMPARE(manager.clientSecret(), QString("test-secret"));
        QCOMPARE(spy.count(), 1);

        spy.clear();
        manager.setClientSecret("test-secret");
        QCOMPARE(manager.clientSecret(), QString("test-secret"));
        QCOMPARE(spy.count(), 0);
    }

    void testSaveAndLoad()
    {
        QString fileName = "test_brewboard_app_settings.ini";
        QSettings::setPath(QSettings::NativeFormat, QSettings::UserScope, QDir::tempPath());

        {
            AppSettingsManager manager;
            manager.setClientId("persisted-id");
            manager.setClientSecret("persisted-secret");
            manager.saveSettings();
        }

        {
            AppSettingsManager manager;
            // Тут проверка дефолтных значений
            QCOMPARE(manager.clientId(), QString(""));
            QCOMPARE(manager.clientSecret(), QString(""));

            manager.loadSettings();

            QCOMPARE(manager.clientId(), QString("persisted-id"));
            QCOMPARE(manager.clientSecret(), QString("persisted-secret"));
        }

        QFile::remove(QDir::tempPath() + "/" + fileName);
    }

    void testSettingsChangedSignal()
    {
        AppSettingsManager manager;
        AppSettingsManagerSpy spy(&manager);

        QCOMPARE(spy.count(), 0);

        manager.setClientId("some-id");
        QCOMPARE(spy.count(), 1);

        manager.setClientSecret("some-secret");
        QCOMPARE(spy.count(), 2);

        manager.setClientId("some-id");
        QCOMPARE(spy.count(), 2);
    }
};

QTEST_MAIN(TestAppSettingsManager)
#include "tst_AppSettingsManager.moc"