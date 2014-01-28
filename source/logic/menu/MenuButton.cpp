#include "MenuButton.h"

#include <QKeyEvent>
#include <rendering/menu/MenuRenderer.h>

MenuButton::MenuButton(const QString & label)
:   m_label(label)
{
}

MenuButton::~MenuButton()
{
}

void MenuButton::accept(MenuRenderer * renderer)
{
    renderer->renderButton(this);
}

void MenuButton::keyPressed(QKeyEvent * event)
{
    if (event->key() == Qt::Key_Return)
        emit pressed();
}

const QString & MenuButton::label() const
{
    return m_label;
}
