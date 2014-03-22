#include "HowtoMenu.h"

#include <logic/menu/items/MenuButton.h>
#include <logic/menu/items/MenuText.h>
#include <logic/menu/items/HowtoItem.h>
#include <logic/menu/items/HowtoListItem.h>

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
    QList<HowtoItem> *  gravity = new QList<HowtoItem>();
    *gravity << HowtoItem("W-Key", "90 degrees (cw)");
    *gravity << HowtoItem("A-Key", "180 degrees");
    *gravity << HowtoItem("S-Key", "90 degrees (ccw)");

    QList<HowtoItem> *  boosts = new QList<HowtoItem>();
    *boosts << HowtoItem("up arrow", "upward boost");
    *boosts << HowtoItem("left arrow key", "leftward boost");
    *boosts << HowtoItem("right arrow key", "rightward boost");
    *boosts << HowtoItem("down arrow key", "downward boost");

    HowtoListItem * keyActionItem = new HowtoListItem(*gravity, *boosts);

    m_toMainMenuButton = new MenuButton("Back to Main Menu");

    return{ keyActionItem, m_toMainMenuButton };
}

void HowtoMenu::connectSignals()
{
    connect(m_toMainMenuButton, &MenuButton::pressed, this, &HowtoMenu::toMainMenuPressed);
}
