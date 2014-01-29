#include "MainMenu.h"

#include "MenuButton.h"

MainMenu::MainMenu()
:   Menu(initializeMenuItems())
{
    connectSignals();
}

MainMenu::~MainMenu()
{
}

QList<MenuItem *> MainMenu::initializeMenuItems()
{
    m_startButton = new MenuButton("Start");
    m_quitButton = new MenuButton("Quit");

    return { m_startButton, m_quitButton };
}

void MainMenu::connectSignals()
{
    connect(m_startButton, &MenuButton::pressed, this, &MainMenu::startPressed);
    connect(m_quitButton, &MenuButton::pressed, this, &MainMenu::quitPressed);
}
