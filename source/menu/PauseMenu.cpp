#include "PauseMenu.h"

#include "MenuButton.h"

PauseMenu::PauseMenu()
:   Menu(initializeMenuItems())
{
    connectSignals();
}

PauseMenu::~PauseMenu()
{
}

QList<MenuItem *> PauseMenu::initializeMenuItems()
{
    m_resumeButton = new MenuButton("Resume");
    m_toMainMenuButton = new MenuButton("Back to Main Menu");

    return { m_resumeButton, m_toMainMenuButton };
}

void PauseMenu::connectSignals()
{
    connect(m_resumeButton, &MenuButton::pressed, this, &PauseMenu::resumePressed);
    connect(m_toMainMenuButton, &MenuButton::pressed, this, &PauseMenu::toMainMenuPressed);
}

