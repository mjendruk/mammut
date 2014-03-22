#pragma once

#include "MenuItem.h"

class QString;

class HowtoItem
{
public:
    HowtoItem(const QString & key, const QString & action);
    virtual ~HowtoItem();

    const QString & key() const;
    const QString & action() const;

protected:
    QString m_key;
    QString m_action;
};
