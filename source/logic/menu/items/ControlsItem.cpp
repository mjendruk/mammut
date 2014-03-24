#include "ControlsItem.h"

#include <QString> 


ControlsItem::ControlsItem(const QString & key, const QString & action)
:   m_key(key)
,   m_action(action)
{
}

ControlsItem::~ControlsItem()
{
}

const QString & ControlsItem::key() const
{
    return m_key;
}

const QString & ControlsItem::action() const
{
    return m_action;
}
