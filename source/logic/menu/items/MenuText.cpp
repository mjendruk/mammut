#include "MenuText.h"

#include <QKeyEvent>
#include <rendering/menu/MenuRenderer.h>
#include <sound/Sound.h>

MenuText::MenuText(const QString & text)
:   m_text(text)
{
}

MenuText::~MenuText()
{
}

void MenuText::accept(MenuRenderer * renderer)
{
    renderer->render(this);
}

void MenuText::keyPressed(QKeyEvent * event)
{    
}

bool MenuText::isFocusable() const
{
    return false;
}

const QString & MenuText::text() const
{
    return m_text;
}
