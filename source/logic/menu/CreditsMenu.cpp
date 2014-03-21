#include "CreditsMenu.h"

#include <logic/menu/items/MenuButton.h>
#include <logic/menu/items/MenuText.h>

CreditsMenu::CreditsMenu()
:   Menu(initializeMenuItems())
{
    connectSignals();
}

CreditsMenu::~CreditsMenu()
{
}

QList<MenuItem *> CreditsMenu::initializeMenuItems()
{
    MenuText * scoreText = new MenuText(QString("Credits"));
    m_toMainMenuButton = new MenuButton("Back to Main Menu");

    return { scoreText, m_toMainMenuButton };
}

void CreditsMenu::connectSignals()
{
    connect(m_toMainMenuButton, &MenuButton::pressed, this, &CreditsMenu::toMainMenuPressed);
}
