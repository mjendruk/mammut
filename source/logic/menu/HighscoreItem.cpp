#include "HighscoreItem.h"

#include <rendering/menu/MenuRenderer.h>

HighscoreItem::HighscoreItem()
{
}

HighscoreItem::~HighscoreItem()
{
}

void HighscoreItem::accept(MenuRenderer * renderer)
{
    renderer->render(this);
}

void HighscoreItem::keyPressed(QKeyEvent * event)
{
}

bool HighscoreItem::isFocusable() const
{
    return false;
}
