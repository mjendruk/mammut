#include "HowtoMenu.h"

#include <logic/menu/items/MenuButton.h>
#include <logic/menu/items/MenuText.h>

HowtoMenu::HowtoMenu()
:   Menu(initializeMenuItems())
{
    connectSignals();
}

HowtoMenu::~HowtoMenu()
{
}

QList<MenuItem *> HowtoMenu::initializeMenuItems()
{
    MenuText * headingText = new MenuText(QString("How-to"));
    m_toMainMenuButton = new MenuButton("Back to Main Menu");

    return{ headingText, m_toMainMenuButton };
}

void HowtoMenu::connectSignals()
{
    connect(m_toMainMenuButton, &MenuButton::pressed, this, &HowtoMenu::toMainMenuPressed);
}
