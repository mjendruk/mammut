#include "MenuInput.h"

#include <QKeyEvent>
#include <rendering/menu/MenuRenderer.h>

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
    if (event->key() == Qt::Key_Backspace)
    {
        m_text.chop(1);
        return;
    }
    
    m_text.append(event->text());
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
    if (m_text.isNull()) return m_placeholder;

    return m_text;
}
