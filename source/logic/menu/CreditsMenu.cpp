#include "CreditsMenu.h"

#include <logic/menu/items/MenuButton.h>
#include <logic/menu/items/MenuText.h>
#include <logic/menu/items/CreditsItem.h>

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
    MenuText * creditsText = new MenuText(QString("Credits"));
    CreditsItem * creditsItem = new CreditsItem();
    m_toMainMenuButton = new MenuButton("Back to Main Menu");

    return{ creditsText, creditsItem, m_toMainMenuButton };
}

void CreditsMenu::connectSignals()
{
    connect(m_toMainMenuButton, &MenuButton::pressed, this, &CreditsMenu::toMainMenuPressed);
}
