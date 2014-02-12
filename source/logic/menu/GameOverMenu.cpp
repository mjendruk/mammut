#include "GameOverMenu.h"

#include <logic/menu/items/MenuButton.h>
#include <logic/menu/items/MenuText.h>

GameOverMenu::GameOverMenu(int score)
:   Menu(initializeMenuItems(score))
{
    connectSignals();
}

GameOverMenu::~GameOverMenu()
{
}

QList<MenuItem *> GameOverMenu::initializeMenuItems(int score)
{
    auto scoreText = new MenuText(QString("Your Score: %1").arg(score));
    m_retryButton = new MenuButton("Retry");
    m_toMainMenuButton = new MenuButton("Back to Main Menu");

    return { scoreText, m_retryButton, m_toMainMenuButton };
}

void GameOverMenu::connectSignals()
{
    connect(m_retryButton, &MenuButton::pressed, this, &GameOverMenu::retryPressed);
    connect(m_toMainMenuButton, &MenuButton::pressed, this, &GameOverMenu::toMainMenuPressed);
}
