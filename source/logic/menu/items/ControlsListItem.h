#pragma once

#include <QList> 
#include "MenuItem.h"

class ControlsItem;

class ControlsListItem : public MenuItem
{
    Q_OBJECT

public:
    ControlsListItem(const QList<ControlsItem> & gravityKeys, const QList<ControlsItem> & boostKeys);
    virtual ~ControlsListItem();

    void accept(MenuRenderer * renderer) override;
    void keyPressed(QKeyEvent * event) override;

    bool isFocusable() const override;

    const QList<ControlsItem> & gravityKeys() const;
    const QList<ControlsItem> & boostKeys() const;

protected:
    QList<ControlsItem> m_gravityKeys;
    QList<ControlsItem> m_boostKeys;

};
