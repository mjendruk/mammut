#include "MenuMechanics.h"

#include <QDebug>
#include <QKeyEvent>
#include "MenuItem.h"

MenuMechanics::MenuMechanics()
:   m_itemIt(m_menuItems)
,   m_renderer(*this)
{
    m_menuItems
        << new MenuItem("Start")
        << new MenuItem("Quit");
    
    m_itemIt = QMutableListIterator<MenuItem *>(m_menuItems);
}

MenuMechanics::~MenuMechanics()
{
    qDeleteAll(m_menuItems);
}

void MenuMechanics::update(float seconds)
{
}

Renderer * MenuMechanics::renderer()
{
    return &m_renderer;
}

void MenuMechanics::keyPressed(QKeyEvent * event)
{
    switch (event->key())
    {
    case Qt::Key_Up:
        if (m_itemIt.hasPrevious()) 
            m_itemIt.previous();
        break;

    case Qt::Key_Down:
        if (m_itemIt.hasNext())
        {
            m_itemIt.next();
            if (!m_itemIt.hasNext())
                m_itemIt.previous();
        }
        break;

    case Qt::Key_Return:
        if (m_itemIt.hasNext())
            m_itemIt.peekNext()->click();
        break;
    }
}

void MenuMechanics::keyReleased(QKeyEvent * event)
{
}

const QList<MenuItem *> MenuMechanics::menuItems() const
{
    return m_menuItems;
}

bool MenuMechanics::isSelectedMenuItem(MenuItem * menuItem) const
{
    return m_itemIt.hasNext() && menuItem == m_itemIt.peekNext();
}
