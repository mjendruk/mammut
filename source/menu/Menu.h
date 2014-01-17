#pragma once

#include <QObject>
#include <QList>

class QKeyEvent;
class MenuItem;

class Menu : public QObject
{
public:
    Menu(QList<MenuItem *> menuItems);
    virtual ~Menu();

    void keyPressed(QKeyEvent * event);

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
