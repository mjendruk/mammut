#include "MainMenu.h"

#include "MenuLogo.h"
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
    m_highscoreButton = new MenuButton("Highscore");
    m_quitButton = new MenuButton("Quit");

    return { new MenuLogo(), m_startButton, m_highscoreButton, m_quitButton };
}

void MainMenu::connectSignals()
{
    connect(m_startButton, &MenuButton::pressed, this, &MainMenu::startPressed);
    connect(m_highscoreButton, &MenuButton::pressed, this, &MainMenu::highscorePressed);
    connect(m_quitButton, &MenuButton::pressed, this, &MainMenu::quitPressed);
}
