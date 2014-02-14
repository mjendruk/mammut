#include "MenuInput.h"

#include <QDebug>
#include <QKeyEvent>

#include <rendering/menu/MenuRenderer.h>
#include <sound/Sound.h>

MenuInput::MenuInput(const QString & label, const QString & placeholder)
:   m_label(label)
,   m_placeholder(placeholder)
{
}

MenuInput::~MenuInput()
{
}

void MenuInput::accept(MenuRenderer * renderer)
{
    renderer->render(this);
}

void MenuInput::keyPressed(QKeyEvent * event)
{
    switch (event->key())
    {
        case Qt::Key_Backspace:
            {
            m_text.chop(1);
            Sound sound(Sound::kTypewriter);
            break;
            } 
        case Qt::Key_Return:
            {
            emit enterPressed(text());
            Sound sound(Sound::kButtonClick);
            break;
            }
        default:
            {
            m_text.append(event->text());
            Sound sound(Sound::kTypewriter);
            break;
            }
    }
}

bool MenuInput::isFocusable() const
{
    return true;
}

const QString & MenuInput::label() const
{
    return m_label;
}

const QString & MenuInput::text() const
{
    if (m_text.isNull()) 
        return m_placeholder;

    return m_text;
}
