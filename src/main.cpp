#include "AppSettingsManager.h"
#include "Controllers/BeerSearchController.h"
#include "LoggerMacros.h"
#include <QDir>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    QCoreApplication::setOrganizationName("Brew Board");
    QCoreApplication::setApplicationName("Brew Board");
    QCoreApplication::setApplicationVersion("0");

    QDir().mkdir("Logs");
    Logger::instance().setLogFile("Logs/log.txt");
    LOG_INFO(QString("Start application %1 v%2")
                 .arg(QCoreApplication::applicationName())
                 .arg(QCoreApplication::applicationVersion()));

    QQmlApplicationEngine engine;

    AppSettingsManager settings;
    settings.loadSettings();

    BeerSearchController searchController;
    searchController.setApiKeys(settings.clientId(), settings.clientSecret());

    QQmlContext* context = engine.rootContext();
    context->setContextProperty("searchController", &searchController);
    context->setContextProperty("appSettings", &settings);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app,
        [url](QObject* obj, const QUrl& objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    qmlRegisterType<BeerListModel>("BrewBoard", 1, 0, "BeerListModel");

    engine.load(QUrl(QStringLiteral("qrc:/main")));

    return app.exec();
}