#include "HighscoreListItem.h"

#include <logic/highscore/HighscoreEntry.h>

#include <rendering/menu/MenuRenderer.h>

HighscoreListItem::HighscoreListItem(const QList<HighscoreEntry> & scores)
:   m_scores(scores)
{
}

HighscoreListItem::~HighscoreListItem()
{
}

void HighscoreListItem::accept(MenuRenderer * renderer)
{
    renderer->render(this);
}

void HighscoreListItem::keyPressed(QKeyEvent * event)
{
}

bool HighscoreListItem::isFocusable() const
{
    return false;
}

const QList<HighscoreEntry> & HighscoreListItem::scores() const
{
    return m_scores;
}
