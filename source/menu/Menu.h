#pragma once

#include <QObject>
#include <QList>
#include <Mechanics.h>

class QKeyEvent;
class MenuItem;

class Menu : public QObject, public Mechanics
{
public:
    Menu(QList<MenuItem *> menuItems);
    virtual ~Menu();

    void update(float seconds) override;
    
    void keyPressed(QKeyEvent * event) override;
    void keyReleased(QKeyEvent * event) override;

    bool isFocusedItem(const MenuItem * menuItem) const;
    
    QList<MenuItem *> menuItems() const;

private:
    void focusPreviousItem();
    void focusNextItem();

    MenuItem * focusedItem() const;

private:
    QList<MenuItem *> m_menuItems;
    unsigned int m_focusIndex;

};
