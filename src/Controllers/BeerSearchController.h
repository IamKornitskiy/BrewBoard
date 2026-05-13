#ifndef BEERSEARCHCONTROLLER_H
#define BEERSEARCHCONTROLLER_H

#include "../Api/UntappdApiClient.h"
#include "../Models/Beer.h"
#include <QObject>
#include <QVector>

class BeerSearchController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector<Beer> beers READ beers NOTIFY beersChanged)
    Q_PROPERTY(bool loading READ loading NOTIFY loadingChanged)

public:
    explicit BeerSearchController(QObject* parent = nullptr);
    QVector<Beer> beers() const { return m_beers; }
    bool loading() const { return m_loading; }

    Q_INVOKABLE void search(const QString& query);
    Q_INVOKABLE void setApiKeys(const QString& clientId, const QString& clientSecret);

signals:
    void beersChanged();
    void loadingChanged();
    void searchError(const QString& error);

private slots:
    void onSearchCompleted(const QVector<Beer>& beers);
    void onErrorOccurred(const QString& error);

private:
    UntappdApiClient* m_apiClient;
    QVector<Beer> m_beers;
    bool m_loading = false;
};

#endif // BEERSEARCHCONTROLLER_H