#ifndef UNTAPPDAPICLIENT_H
#define UNTAPPDAPICLIENT_H

#include "../Models/Beer.h"
#include <QNetworkAccessManager>
#include <QObject>
#include <QVector>

class UntappdApiClient : public QObject
{
    Q_OBJECT

public:
    explicit UntappdApiClient(QObject* parent = nullptr);
    explicit UntappdApiClient(QNetworkAccessManager* nam, QObject* parent = nullptr);
    void setClientId(const QString& id) { m_clientId = id; }
    void setClientSecret(const QString& secret) { m_clientSecret = secret; }

    void searchBeer(const QString& query, int limit = 10);

signals:
    void searchCompleted(const QVector<Beer>& beers);
    void errorOccurred(const QString& error);
    void rateLimitInfo(int limit, int remaining);

private slots:
    void onSearchFinished();

private:
    QNetworkAccessManager* m_nam;
    QNetworkReply* m_currentReply;
    QString m_clientId;
    QString m_clientSecret;

    QVector<Beer> parseSearchResponse(const QByteArray& data);
    void handleError(const QString& error);
};

#endif // UNTAPPDAPICLIENT_H