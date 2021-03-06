#pragma once

#include "Menu.h"

class MenuButton;

class MainMenu : public Menu
{
    Q_OBJECT

public:
    MainMenu();
    virtual ~MainMenu();

signals:
    void startPressed();
    void highscorePressed();
    void controlsPressed();
    void creditsPressed();
    void quitPressed();

protected:
    QList<MenuItem *> initializeMenuItems();
    void connectSignals();

protected:
    MenuButton * m_startButton;
    MenuButton * m_highscoreButton;
    MenuButton * m_controlsButton;
    MenuButton * m_creditsButton;
    MenuButton * m_quitButton;

};
