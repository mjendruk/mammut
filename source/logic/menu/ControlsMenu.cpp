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
    *gravity << ControlsItem("A", "90 degrees (cw)");
    *gravity << ControlsItem("W", "180 degrees");
    *gravity << ControlsItem("D", "90 degrees (ccw)");

    QList<ControlsItem> *  boosts = new QList<ControlsItem>();
    *boosts << ControlsItem("arrow keys", "Apply Boost");
    *boosts << ControlsItem("ESC", "Pause Menu");

    ControlsListItem * keyActionItem = new ControlsListItem(*gravity, *boosts);

    m_toMainMenuButton = new MenuButton("Back");

    return{ keyActionItem, m_toMainMenuButton };
}

void ControlsMenu::connectSignals()
{
    connect(m_toMainMenuButton, &MenuButton::pressed, this, &ControlsMenu::toMainMenuPressed);
}
