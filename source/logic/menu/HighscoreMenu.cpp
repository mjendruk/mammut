#include "HighscoreMenu.h"

#include "MenuButton.h"

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
    m_backButton = new MenuButton("Back");

    return { m_backButton };
}

void HighscoreMenu::connectSignals()
{
    connect(m_backButton, &MenuButton::pressed, this, &HighscoreMenu::backPressed);
}
