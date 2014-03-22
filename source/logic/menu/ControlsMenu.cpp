#include "ControlsMenu.h"

#include <logic/menu/items/MenuButton.h>
#include <logic/menu/items/MenuText.h>
#include <logic/menu/items/ControlsItem.h>
#include <logic/menu/items/ControlsListItem.h>

ControlsMenu::ControlsMenu()
:   Menu(initializeMenuItems())
{
    connectSignals();
}

ControlsMenu::~ControlsMenu()
{
}

QList<MenuItem *> ControlsMenu::initializeMenuItems()
{
    QList<ControlsItem> *  gravity = new QList<ControlsItem>();
    *gravity << ControlsItem("W", "90 degrees (cw)");
    *gravity << ControlsItem("A", "180 degrees");
    *gravity << ControlsItem("S", "90 degrees (ccw)");

    QList<ControlsItem> *  boosts = new QList<ControlsItem>();
    *boosts << ControlsItem("up arrow", "upward boost");
    *boosts << ControlsItem("left arrow", "leftward boost");
    *boosts << ControlsItem("right arrow", "rightward boost");
    *boosts << ControlsItem("down arrow", "downward boost");

    ControlsListItem * keyActionItem = new ControlsListItem(*gravity, *boosts);

    m_toMainMenuButton = new MenuButton("Back");

    return{ keyActionItem, m_toMainMenuButton };
}

void ControlsMenu::connectSignals()
{
    connect(m_toMainMenuButton, &MenuButton::pressed, this, &ControlsMenu::toMainMenuPressed);
}
