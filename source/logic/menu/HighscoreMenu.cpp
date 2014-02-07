#include "HighscoreMenu.h"

#include "MenuButton.h"
#include "HighscoreItem.h"

HighscoreMenu::HighscoreMenu()
:   Menu(initializeMenuItems())
{
    connectSignals();
}

HighscoreMenu::~HighscoreMenu()
{
}

QList<MenuItem *> HighscoreMenu::initializeMenuItems()
{
    m_highscore = new HighscoreItem();
    m_backButton = new MenuButton("Back");

    return { m_highscore, m_backButton };
}

void HighscoreMenu::connectSignals()
{
    connect(m_backButton, &MenuButton::pressed, this, &HighscoreMenu::backPressed);
}
