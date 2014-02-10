#include "MenuInput.h"

#include <QKeyEvent>
#include <rendering/menu/MenuRenderer.h>

MenuInput::MenuInput(const QString & label)
:   m_label(label)
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
    return m_text;
}
