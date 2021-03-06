#include "MenuInput.h"

#include <QDebug>
#include <QKeyEvent>

#include <rendering/menu/MenuRenderer.h>
#include <sound/Sound.h>

MenuInput::MenuInput(const QString & label, const QString & initialText)
:   m_label(label)
,   m_text(initialText)
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
            for (QChar c : event->text()) {
                if (c.isDigit() || c.isLetter()) {
                    m_text.append(c);
                    Sound sound(Sound::kTypewriter);
                }
            }
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
    return m_text;
}
