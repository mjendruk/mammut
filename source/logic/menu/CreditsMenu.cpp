#include "CreditsMenu.h"

#include <logic/menu/items/MenuButton.h>
#include <logic/menu/items/MenuText.h>
#include <logic/menu/items/CreditsListItem.h>

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
    CreditsListItem * creditsListItem = new CreditsListItem();
    //MenuText * thanksText = new MenuText(QString("Thanks to"));
    m_toMainMenuButton = new MenuButton("Back to Main Menu");

    return{ creditsText, creditsListItem, m_toMainMenuButton };
}

void CreditsMenu::connectSignals()
{
    connect(m_toMainMenuButton, &MenuButton::pressed, this, &CreditsMenu::toMainMenuPressed);
}
