#include "Beer.h"
#include "UntappdApiClient.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSignalSpy>
#include <QTimer>
#include <QtTest>

class MockNetworkReply : public QNetworkReply
{
    Q_OBJECT
public:
    MockNetworkReply(const QByteArray& data, const QMap<QByteArray, QByteArray>& headers,
                     QNetworkReply::NetworkError errorCode = QNetworkReply::NoError,
                     const QString& errorString = QString())
        : QNetworkReply(nullptr)
    {
        m_data = data;
        for (auto it = headers.cbegin(); it != headers.cend(); ++it) {
            setRawHeader(it.key(), it.value());
        }
        setError(errorCode, errorString);
        open(QIODevice::ReadOnly);
        QTimer::singleShot(0, this, [this]() { emit finished(); });
    }

    qint64 readData(char* data, qint64 maxlen) override
    {
        qint64 size = qMin(maxlen, (qint64)m_data.size());
        memcpy(data, m_data.constData(), size);
        m_data.remove(0, size);
        return size;
    }

    void abort() override {}

private:
    QByteArray m_data;
};

class MockNetworkAccessManager : public QNetworkAccessManager
{
public:
    MockNetworkReply* m_expectedReply = nullptr;

protected:
    QNetworkReply* createRequest(Operation op, const QNetworkRequest& request,
                                 QIODevice* outgoingData) override
    {
        Q_UNUSED(op)
        Q_UNUSED(request)
        Q_UNUSED(outgoingData)
        if (m_expectedReply) {
            m_expectedReply->setParent(this);
            MockNetworkReply* reply = m_expectedReply;
            m_expectedReply = nullptr;
            return reply;
        }
        return new MockNetworkReply(QByteArray(), {});
    }
};

class TestUntappdApiClient : public QObject
{
    Q_OBJECT

private:
    MockNetworkAccessManager* mockNam;
    UntappdApiClient* client;

    void initValidClient()
    {
        client->setClientId("test_id");
        client->setClientSecret("test_secret");
    }

private slots:
    void initTestCase() { qRegisterMetaType<QVector<Beer>>("QVector<Beer>"); }

    void init()
    {
        mockNam = new MockNetworkAccessManager;
        client = new UntappdApiClient(mockNam);
        mockNam->setParent(client);
    }

    void cleanup()
    {
        delete client;
        QCoreApplication::processEvents();
    }

    void testMissingCredentials()
    {
        QSignalSpy errorSpy(client, &UntappdApiClient::errorOccurred);
        client->searchBeer("test");
        QCOMPARE(errorSpy.count(), 1);
        QVERIFY(errorSpy.at(0).at(0).toString().contains("Client ID or Secret"));
        QVERIFY(!mockNam->m_expectedReply);
    }

    void testSuccessfulSearch()
    {
        initValidClient();

        QByteArray json = R"({
                "meta": {"code": 200},
                "response": {
                    "beers": {
                        "count": 1,
                        "items": [
                            {
                                "beer": {
                                    "bid": 123,
                                    "beer_name": "Pale Ale",
                                    "brewery": {"brewery_name": "BrewCo"},
                                    "beer_abv": 5.5,
                                    "beer_ibu": 45,
                                    "beer_description": "A nice beer",
                                    "beer_label": "label.png"
                                }
                            }
                        ]
                    }
                }
            })";

        QMap<QByteArray, QByteArray> headers;
        headers["X-Ratelimit-Limit"] = "100";
        headers["X-Ratelimit-Remaining"] = "99";

        mockNam->m_expectedReply = new MockNetworkReply(json, headers);

        QSignalSpy completeSpy(client, &UntappdApiClient::searchCompleted);
        QSignalSpy rateSpy(client, &UntappdApiClient::rateLimitInfo);
        QSignalSpy errorSpy(client, &UntappdApiClient::errorOccurred);

        client->searchBeer("pale ale");

        QVERIFY(completeSpy.wait(1000));
        QCOMPARE(completeSpy.count(), 1);
        QCOMPARE(errorSpy.count(), 0);

        QCOMPARE(rateSpy.count(), 1);
        QList<QVariant> rateArgs = rateSpy.takeFirst();
        QCOMPARE(rateArgs.at(0).toInt(), 100);
        QCOMPARE(rateArgs.at(1).toInt(), 99);

        QVector<Beer> beers = completeSpy.at(0).at(0).value<QVector<Beer>>();
        QCOMPARE(beers.size(), 1);
        Beer b = beers.first();
        QCOMPARE(b.id(), 123);
        QCOMPARE(b.name(), "Pale Ale");
        QCOMPARE(b.brewery(), "BrewCo");
        QCOMPARE(b.abv(), 5.5);
        QCOMPARE(b.ibu(), 45);
        QCOMPARE(b.description(), "A nice beer");
        QCOMPARE(b.labelUrl(), QUrl("label.png"));
    }

    void testNetworkError()
    {
        initValidClient();

        MockNetworkReply* reply = new MockNetworkReply(
            QByteArray(), {}, QNetworkReply::ConnectionRefusedError, "Connection refused");
        mockNam->m_expectedReply = reply;

        QSignalSpy errorSpy(client, &UntappdApiClient::errorOccurred);
        QSignalSpy completeSpy(client, &UntappdApiClient::searchCompleted);

        client->searchBeer("test");
        QVERIFY(errorSpy.wait(1000));
        QCOMPARE(errorSpy.count(), 1);
        QCOMPARE(completeSpy.count(), 1);
        QVERIFY(completeSpy.at(0).at(0).value<QVector<Beer>>().isEmpty());
    }

    void testApiError()
    {
        initValidClient();

        QByteArray json = R"({
            "meta": {"code": 500, "error_detail": "Internal Server Error"}
        })";

        mockNam->m_expectedReply = new MockNetworkReply(json, {});

        QSignalSpy errorSpy(client, &UntappdApiClient::errorOccurred);
        QSignalSpy completeSpy(client, &UntappdApiClient::searchCompleted);

        client->searchBeer("test");
        QVERIFY(errorSpy.wait(1000));
        QCOMPARE(errorSpy.count(), 1);
        QVERIFY(errorSpy.at(0).at(0).toString().contains("Internal Server Error"));
        QCOMPARE(completeSpy.count(), 1);
        QVERIFY(completeSpy.at(0).at(0).value<QVector<Beer>>().isEmpty());
    }

    void testInvalidJson()
    {
        initValidClient();

        QByteArray invalid = "not a json";
        mockNam->m_expectedReply = new MockNetworkReply(invalid, {});

        QSignalSpy errorSpy(client, &UntappdApiClient::errorOccurred);
        QSignalSpy completeSpy(client, &UntappdApiClient::searchCompleted);

        client->searchBeer("test");
        QVERIFY(errorSpy.wait(1000));
        QCOMPARE(errorSpy.count(), 1);
        QVERIFY(errorSpy.at(0).at(0).toString().contains("Invalid JSON"));
        QCOMPARE(completeSpy.count(), 1);
        QVERIFY(completeSpy.at(0).at(0).value<QVector<Beer>>().isEmpty());
    }

    void testMissingRateLimitHeaders()
    {
        initValidClient();

        QByteArray json = R"({"meta":{"code":200},"response":{"beers":{"count":0,"items":[]}}})";
        mockNam->m_expectedReply = new MockNetworkReply(json, {}); // нет заголовков

        QSignalSpy rateSpy(client, &UntappdApiClient::rateLimitInfo);
        QSignalSpy completeSpy(client, &UntappdApiClient::searchCompleted);

        client->searchBeer("test");
        QVERIFY(completeSpy.wait(1000));
        QCOMPARE(completeSpy.count(), 1);
        QCOMPARE(rateSpy.count(), 0);
    }
};

QTEST_MAIN(TestUntappdApiClient)
#include "tst_UntappdApiClient.moc"