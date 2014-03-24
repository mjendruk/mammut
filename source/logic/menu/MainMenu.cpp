#include "MainMenu.h"

#include <logic/menu/items/MenuLogo.h>
#include <logic/menu/items/MenuButton.h>

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
    m_highscoreButton = new MenuButton("Highscore");
    m_controlsButton = new MenuButton("Controls");
    m_creditsButton = new MenuButton("Credits");
    m_quitButton = new MenuButton("Quit");

    return{ new MenuLogo(), m_startButton, m_highscoreButton, m_controlsButton, m_creditsButton, m_quitButton };
}

void MainMenu::connectSignals()
{
    connect(m_startButton, &MenuButton::pressed, this, &MainMenu::startPressed);
    connect(m_highscoreButton, &MenuButton::pressed, this, &MainMenu::highscorePressed);
    connect(m_controlsButton, &MenuButton::pressed, this, &MainMenu::controlsPressed);
    connect(m_creditsButton, &MenuButton::pressed, this, &MainMenu::creditsPressed);
    connect(m_quitButton, &MenuButton::pressed, this, &MainMenu::quitPressed);
}
