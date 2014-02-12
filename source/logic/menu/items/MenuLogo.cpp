#include "MenuLogo.h"

#include <rendering/menu/MenuRenderer.h>

MenuLogo::MenuLogo()
{
}

MenuLogo::~MenuLogo()
{
}

void MenuLogo::accept(MenuRenderer * renderer)
{
    renderer->render(this);
}

void MenuLogo::keyPressed(QKeyEvent * event)
{
}

bool MenuLogo::isFocusable() const
{
    return false;
}
