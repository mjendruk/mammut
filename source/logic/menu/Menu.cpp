#include "Menu.h"

#include <cassert>
#include <QKeyEvent>
#include <sound/Sound.h>
#include "MenuItem.h"

Menu::Menu(QList<MenuItem *> menuItems)
:   m_menuItems(menuItems)
,   m_focusIndex(0)
{
    assert(!menuItems.isEmpty());
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
    Sound sound(Sound::kButtonSwitch, true);
    
    switch (event->key())
    {
    case Qt::Key_Up:
        sound.setPaused(false);
        focusPreviousItem();
        break;

    case Qt::Key_Down:
        sound.setPaused(false);
        focusNextItem();
        break;

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
}

void Menu::focusNextItem()
{
    m_focusIndex = m_focusIndex == m_menuItems.count() - 1 ? 0 : m_focusIndex + 1;
}

MenuItem * Menu::focusedItem() const
{
    return m_menuItems[m_focusIndex];
}
