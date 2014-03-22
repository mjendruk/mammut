#include "HowtoItem.h"

#include <QString> 


HowtoItem::HowtoItem(const QString & key, const QString & action)
:   m_key(key)
,   m_action(action)
{
}

HowtoItem::~HowtoItem()
{
}

const QString & HowtoItem::key() const
{
    return m_key;
}

const QString & HowtoItem::action() const
{
    return m_action;
}
