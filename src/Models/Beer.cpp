#include "Beer.h"

Beer::Beer(int id, const QString &name, const QString &brewery,
           double abv, int ibu, const QString &description, const QUrl &labelUrl)
    : m_id(id)
    , m_name(name)
    , m_brewery(brewery)
    , m_abv(abv)
    , m_ibu(ibu)
    , m_description(description)
    , m_labelUrl(labelUrl)
{
}