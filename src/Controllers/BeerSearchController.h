#ifndef BEERSEARCHCONTROLLER_H
#define BEERSEARCHCONTROLLER_H

#include "../Api/UntappdApiClient.h"
#include "../Models/BeerListModel.h"
#include <QObject>
#include <QVector>

class BeerSearchController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(BeerListModel* beers READ beers NOTIFY beersChanged) // было model -> beers
    Q_PROPERTY(bool loading READ loading NOTIFY loadingChanged)

public:
    explicit BeerSearchController(QObject* parent = nullptr);
    BeerListModel* beers() const { return m_beers; } // возвращает модель
    bool loading() const { return m_loading; }

    Q_INVOKABLE void search(const QString& query);
    Q_INVOKABLE void setApiKeys(const QString& clientId, const QString& clientSecret);

signals:
    void beersChanged(); // вместо modelChanged
    void loadingChanged();
    void searchError(const QString& error);

private slots:
    void onSearchCompleted(const QVector<Beer>& beers);
    void onErrorOccurred(const QString& error);

private:
    UntappdApiClient* m_apiClient;
    BeerListModel* m_beers; // модель для списка пива
    bool m_loading = false;
};

#endif // BEERSEARCHCONTROLLER_H