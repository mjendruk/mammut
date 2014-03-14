#include "MenuButton.h"

#include <QKeyEvent>
#include <rendering/menu/MenuRenderer.h>
#include <sound/Sound.h>

MenuButton::MenuButton(const QString & label)
:   m_label(label)
{
}

MenuButton::~MenuButton()
{
}

void MenuButton::accept(MenuRenderer * renderer)
{
    renderer->render(this);
}

void MenuButton::keyPressed(QKeyEvent * event)
{
    if (event->key() == Qt::Key_Return)
    {
        Sound sound(Sound::kButtonClick);
        emit pressed();
    }
    
}

bool MenuButton::isFocusable() const
{
    return true;
}

const QString & MenuButton::label() const
{
    return m_label;
}
