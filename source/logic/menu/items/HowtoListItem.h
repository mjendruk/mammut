#pragma once

#include <QList> 
#include "MenuItem.h"

class HowtoItem;

class HowtoListItem : public MenuItem
{
    Q_OBJECT

public:
    HowtoListItem(const QList<HowtoItem> & gravityKeys, const QList<HowtoItem> & boostKeys);
    virtual ~HowtoListItem();

    void accept(MenuRenderer * renderer) override;
    void keyPressed(QKeyEvent * event) override;

    bool isFocusable() const override;

    const QList<HowtoItem> & gravityKeys() const;
    const QList<HowtoItem> & boostKeys() const;

protected:
    QList<HowtoItem> m_gravityKeys;
    QList<HowtoItem> m_boostKeys;

};
