#include "HighscoreMenu.h"

#include <logic/menu/items/MenuButton.h>
#include <logic/menu/items/HighscoreListItem.h>

HighscoreMenu::HighscoreMenu(const QList<HighscoreEntry> & scores)
:   Menu(initializeMenuItems(scores))
{
    connectSignals();
}

HighscoreMenu::~HighscoreMenu()
{
}

QList<MenuItem *> HighscoreMenu::initializeMenuItems(const QList<HighscoreEntry> & scores)
{
    m_highscore = new HighscoreListItem(scores);
    m_backButton = new MenuButton("Back");

    return { m_highscore, m_backButton };
}

void HighscoreMenu::connectSignals()
{
    connect(m_backButton, &MenuButton::pressed, this, &HighscoreMenu::backPressed);
}
