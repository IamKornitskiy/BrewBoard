#ifndef BEER_H
#define BEER_H

#include <QString>
#include <QUrl>

class Beer
{
public:
    Beer() = default;
    Beer(int id, const QString &name, const QString &brewery,
         double abv, int ibu, const QString &description, const QUrl &labelUrl);

    int id() const { return m_id; }
    QString name() const { return m_name; }
    QString brewery() const { return m_brewery; }
    double abv() const { return m_abv; }
    int ibu() const { return m_ibu; }
    QString description() const { return m_description; }
    QUrl labelUrl() const { return m_labelUrl; }

private:
    int m_id = 0;
    QString m_name;
    QString m_brewery;
    double m_abv = 0.0;
    int m_ibu = 0;
    QString m_description;
    QUrl m_labelUrl;
};

#endif // BEER_H