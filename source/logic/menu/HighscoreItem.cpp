#include "HighscoreItem.h"

#include <logic/highscore/HighscoreEntry.h>

#include <rendering/menu/MenuRenderer.h>

HighscoreItem::HighscoreItem(const QList<HighscoreEntry> & scores)
:   m_scores(scores)
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

const QList<HighscoreEntry> & HighscoreItem::scores() const
{
    return m_scores;
}
