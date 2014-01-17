#include "MenuMechanics.h"

#include <cassert>
#include <QDebug>
#include <QKeyEvent>
#include "Menu.h"

MenuMechanics::MenuMechanics(Menu * menu)
:   m_menu(menu)
,   m_renderer(*this)
{
    assert(menu != nullptr);
}

MenuMechanics::~MenuMechanics()
{
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
    m_menu->keyPressed(event);
}

void MenuMechanics::keyReleased(QKeyEvent * event)
{
}

const Menu & MenuMechanics::menu() const
{
    return *m_menu;
}
