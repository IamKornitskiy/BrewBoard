#include "BeerSearchController.h"
#include <QDebug>

BeerSearchController::BeerSearchController(QObject* parent)
    : QObject(parent)
    , m_apiClient(new UntappdApiClient(this))
    , m_beers(new BeerListModel(this))
{
    connect(m_apiClient, &UntappdApiClient::searchCompleted, this,
            &BeerSearchController::onSearchCompleted);
    connect(m_apiClient, &UntappdApiClient::errorOccurred, this,
            &BeerSearchController::onErrorOccurred);
}

void BeerSearchController::search(const QString& query)
{
    if (query.trimmed().isEmpty()) {
        qWarning() << "Empty search query";
        return;
    }
    if (m_loading) {
        qDebug() << "Search already in progress";
        return;
    }
    m_loading = true;
    emit loadingChanged();
    m_apiClient->searchBeer(query);
}

void BeerSearchController::setApiKeys(const QString& clientId, const QString& clientSecret)
{
    m_apiClient->setClientId(clientId);
    m_apiClient->setClientSecret(clientSecret);
    qDebug() << "API keys set";
}

void BeerSearchController::onSearchCompleted(const QVector<Beer>& beers)
{
    m_beers->setBeers(beers);
    m_loading = false;
    emit beersChanged();
    emit loadingChanged();
}

void BeerSearchController::onErrorOccurred(const QString& error)
{
    m_loading = false;
    emit loadingChanged();
    emit searchError(error);
    qDebug() << "Search error:" << error;
}