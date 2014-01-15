#pragma once

#include <QList>
#include <QMutableListIterator>
#include <Mechanics.h>
#include "MenuRenderer.h"

class QKeyEvent;
class MenuItem;

class MenuMechanics : public Mechanics
{
public:
    MenuMechanics();
    virtual ~MenuMechanics();

    virtual void update(float seconds);
    virtual Renderer * renderer();

    void keyPressed(QKeyEvent * event) override;
    void keyReleased(QKeyEvent * event) override;

    const QList<MenuItem *> menuItems() const;
    bool isSelectedMenuItem(MenuItem * menuItem) const;

protected:
    QList<MenuItem *> m_menuItems;
    QMutableListIterator<MenuItem *> m_itemIt;

    MenuRenderer m_renderer;
    
};
