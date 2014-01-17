#include "StartMenu.h"

#include "MenuButton.h"

StartMenu::StartMenu()
:   Menu(initializeMenuItems())
{
    connectSignals();
}

StartMenu::~StartMenu()
{
}

QList<MenuItem *> StartMenu::initializeMenuItems()
{
    m_startButton = new MenuButton("Start");
    m_quitButton = new MenuButton("Quit");

    return { m_startButton, m_quitButton };
}

void StartMenu::connectSignals()
{
    connect(m_startButton, &MenuButton::pressed, this, &StartMenu::startPressed);
    connect(m_quitButton, &MenuButton::pressed, this, &StartMenu::quitPressed);
}
