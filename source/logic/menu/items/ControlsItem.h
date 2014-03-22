#pragma once

#include "MenuItem.h"

class QString;

class ControlsItem
{
public:
    ControlsItem(const QString & key, const QString & action);
    virtual ~ControlsItem();

    const QString & key() const;
    const QString & action() const;

protected:
    QString m_key;
    QString m_action;
};
