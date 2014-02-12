#pragma once

#include "Menu.h"

class MenuButton;
class MenuText;

class GameOverMenu : public Menu
{
    Q_OBJECT

public:
    GameOverMenu(int score);
    virtual ~GameOverMenu();

signals:
    void retryPressed();
    void toMainMenuPressed();

protected:
    QList<MenuItem *> initializeMenuItems(int score);
    void connectSignals();

protected:
    MenuButton * m_retryButton;
    MenuButton * m_toMainMenuButton;

};
