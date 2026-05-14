#ifndef BEERLISTMODEL_H
#define BEERLISTMODEL_H

#include "Beer.h"
#include <QAbstractListModel>
#include <QVector>

class BeerListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        NameRole,
        BreweryRole,
        AbvRole,
        IbuRole,
        DescriptionRole,
        LabelUrlRole
    };

    explicit BeerListModel(QObject* parent = nullptr);
    void setBeers(const QVector<Beer>& beers);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    QVector<Beer> m_beers;
};

#endif // BEERLISTMODEL_H