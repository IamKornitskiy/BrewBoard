#include "UntappdApiClient.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>

UntappdApiClient::UntappdApiClient(QObject* parent)
    : UntappdApiClient(new QNetworkAccessManager, parent)
{
}

UntappdApiClient::UntappdApiClient(QNetworkAccessManager* nam, QObject* parent)
    : QObject(parent)
    , m_nam(nam ? nam : new QNetworkAccessManager(this))
    , m_currentReply(nullptr)
{
}

void UntappdApiClient::searchBeer(const QString& query, int limit)
{
    if (m_clientId.isEmpty() || m_clientSecret.isEmpty()) {
        handleError("Client ID or Secret is missing");
        return;
    }

    QUrl url("https://api.untappd.com/v4/search/beer");
    QUrlQuery queryParams;
    queryParams.addQueryItem("client_id", m_clientId);
    queryParams.addQueryItem("client_secret", m_clientSecret);
    queryParams.addQueryItem("q", query);
    queryParams.addQueryItem("limit", QString::number(limit));
    url.setQuery(queryParams);

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader,
                      "BrewBoard/1.0 (client_id=" + m_clientId + ")");

    qDebug() << "Searching Untappd for:" << query;
    m_currentReply = m_nam->get(request);
    connect(m_currentReply, &QNetworkReply::finished, this, &UntappdApiClient::onSearchFinished);
}

void UntappdApiClient::onSearchFinished()
{
    if (!m_currentReply)
        return;

    QVector<Beer> beers;
    if (m_currentReply->error() == QNetworkReply::NoError) {
        QByteArray data = m_currentReply->readAll();
        beers = parseSearchResponse(data);

        // Извлечение заголовков rate limit (опционально)
        bool limitOk = false, remainingOk = false;
        int limit = m_currentReply->rawHeader("X-Ratelimit-Limit").toInt(&limitOk);
        int remaining = m_currentReply->rawHeader("X-Ratelimit-Remaining").toInt(&remainingOk);
        if (limitOk && remainingOk) {
            emit rateLimitInfo(limit, remaining);
        }
    }
    else {
        handleError(m_currentReply->errorString());
    }

    m_currentReply->deleteLater();
    m_currentReply = nullptr;
    emit searchCompleted(beers);
}

QVector<Beer> UntappdApiClient::parseSearchResponse(const QByteArray& data)
{
    QVector<Beer> beers;
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        handleError("Invalid JSON response");
        return beers;
    }

    QJsonObject root = doc.object();
    QJsonObject meta = root["meta"].toObject();
    int code = meta["code"].toInt();
    if (code != 200) {
        QString error = meta["developer_friendly"].toString();
        if (error.isEmpty())
            error = meta["error_detail"].toString();
        handleError(error);
        return beers;
    }

    QJsonObject response = root["response"].toObject();
    QJsonObject beersObj = response["beers"].toObject();
    QJsonArray items = beersObj["items"].toArray();

    for (const QJsonValue& item : items) {
        QJsonObject beerObj = item["beer"].toObject();
        int id = beerObj["bid"].toInt();
        QString name = beerObj["beer_name"].toString();
        QString brewery = beerObj["brewery"].toObject()["brewery_name"].toString();
        double abv = beerObj["beer_abv"].toDouble();
        int ibu = beerObj["beer_ibu"].toInt();
        QString description = beerObj["beer_description"].toString();
        QUrl labelUrl = beerObj["beer_label"].toString();

        beers.append(Beer(id, name, brewery, abv, ibu, description, labelUrl));
    }

    qDebug() << "Found" << beers.size() << "beers";
    return beers;
}

void UntappdApiClient::handleError(const QString& error)
{
    qWarning() << "Untappd API error:" << error;
    emit errorOccurred(error);
}