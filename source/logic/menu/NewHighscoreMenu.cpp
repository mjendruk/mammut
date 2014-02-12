#include "NewHighscoreMenu.h"

#include <QString>

#include <logic/menu/items/MenuInput.h>
#include <logic/menu/items/MenuText.h>


NewHighscoreMenu::NewHighscoreMenu(const QString & cachedName, int score)
:   Menu(initializeMenuItems(cachedName, score))
{
    connectSignals();
}

NewHighscoreMenu::~NewHighscoreMenu()
{
}

QList<MenuItem *> NewHighscoreMenu::initializeMenuItems(const QString & cachedName, int score)
{
    auto scoreText = new MenuText(QString("Congrats! New Highscore: %1").arg(score));
    m_input = new MenuInput("Name", cachedName);

    return { scoreText, m_input };
}

void NewHighscoreMenu::connectSignals()
{
    connect(m_input, &MenuInput::enterPressed, this, &NewHighscoreMenu::nameEntered);
}
