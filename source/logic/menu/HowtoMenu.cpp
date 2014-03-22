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
    *gravity << HowtoItem("W", "90 degrees (cw)");
    *gravity << HowtoItem("A", "180 degrees");
    *gravity << HowtoItem("S", "90 degrees (ccw)");

    QList<HowtoItem> *  boosts = new QList<HowtoItem>();
    *boosts << HowtoItem("up arrow", "upward boost");
    *boosts << HowtoItem("left arrow", "leftward boost");
    *boosts << HowtoItem("right arrow", "rightward boost");
    *boosts << HowtoItem("down arrow", "downward boost");

    HowtoListItem * keyActionItem = new HowtoListItem(*gravity, *boosts);

    m_toMainMenuButton = new MenuButton("Back");

    return{ keyActionItem, m_toMainMenuButton };
}

void HowtoMenu::connectSignals()
{
    connect(m_toMainMenuButton, &MenuButton::pressed, this, &HowtoMenu::toMainMenuPressed);
}
