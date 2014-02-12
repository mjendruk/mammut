#include "NewHighscoreMenu.h"

#include <QString>

#include "MenuButton.h"
#include "MenuInput.h"
#include "MenuText.h"


NewHighscoreMenu::NewHighscoreMenu(const QString & cachedName, int score)
:   Menu(initializeMenuItems(cachedName, score))
{
    connectSignals();
}

NewHighscoreMenu::~NewHighscoreMenu()
{
}

void NewHighscoreMenu::onOkPressed()
{
    emit nameEntered(m_input->text());
}

QList<MenuItem *> NewHighscoreMenu::initializeMenuItems(const QString & cachedName, int score)
{
    auto scoreText = new MenuText(QString("Congrats! New Highscore: %1").arg(score));
    m_input = new MenuInput("Name", cachedName);
    m_okButton = new MenuButton("Ok");

    return { scoreText, m_input, m_okButton };
}

void NewHighscoreMenu::connectSignals()
{
    connect(m_okButton, &MenuButton::pressed, this, &NewHighscoreMenu::onOkPressed);
}
