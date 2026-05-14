#include "BeerListModel.h"

BeerListModel::BeerListModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

void BeerListModel::setBeers(const QVector<Beer>& beers)
{
    beginResetModel();
    m_beers = beers;
    endResetModel();
}

int BeerListModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;
    return m_beers.size();
}

QVariant BeerListModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_beers.size())
        return QVariant();

    const Beer& beer = m_beers.at(index.row());
    switch (role) {
    case IdRole:
        return beer.id();
    case NameRole:
        return beer.name();
    case BreweryRole:
        return beer.brewery();
    case AbvRole:
        return beer.abv();
    case IbuRole:
        return beer.ibu();
    case DescriptionRole:
        return beer.description();
    case LabelUrlRole:
        return beer.labelUrl();
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> BeerListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[NameRole] = "name";
    roles[BreweryRole] = "brewery";
    roles[AbvRole] = "abv";
    roles[IbuRole] = "ibu";
    roles[DescriptionRole] = "description";
    roles[LabelUrlRole] = "labelUrl";
    return roles;
}