#include "Menu.h"

#include <cassert>

#include <QKeyEvent>

#include <sound/Sound.h>

#include <logic/menu/items/MenuItem.h>

Menu::Menu(QList<MenuItem *> menuItems)
:   m_menuItems(menuItems)
,   m_focusIndex(0)
{
    assert(!menuItems.isEmpty());

    if (!focusedItem()->isFocusable())
        focusNextItem();
}

Menu::~Menu()
{
    qDeleteAll(m_menuItems);
}

void Menu::update(float seconds)
{
}

void Menu::keyPressed(QKeyEvent * event)
{   
    switch (event->key())
    {
    case Qt::Key_Up:
        {
        Sound sound(Sound::kButtonSwitch);
        focusPreviousItem();
        break;
        }  

    case Qt::Key_Down:
        {
        Sound sound(Sound::kButtonSwitch);
        focusNextItem();
        break;
        }

    default:
        focusedItem()->keyPressed(event);
        break;
    }
}

void Menu::keyReleased(QKeyEvent * event)
{
}

QList<MenuItem *> Menu::menuItems() const
{
    return m_menuItems;
}

bool Menu::isFocusedItem(const MenuItem * menuItem) const
{
    return focusedItem() == menuItem;
}

void Menu::focusPreviousItem()
{
    m_focusIndex = m_focusIndex == 0 ? m_menuItems.count() - 1 : m_focusIndex - 1;

    if (!focusedItem()->isFocusable())
        focusPreviousItem();
}

void Menu::focusNextItem()
{
    m_focusIndex = m_focusIndex == m_menuItems.count() - 1 ? 0 : m_focusIndex + 1;

    if (!focusedItem()->isFocusable())
        focusNextItem();
}

MenuItem * Menu::focusedItem() const
{
    return m_menuItems[m_focusIndex];
}
