#pragma once

#include "Menu.h"

class MenuButton;

class StartMenu : public Menu
{
    Q_OBJECT

public:
    StartMenu();
    virtual ~StartMenu();

signals:
    void startPressed();
    void quitPressed();

protected:
    QList<MenuItem *> initializeMenuItems();
    void connectSignals();

protected:
    MenuButton * m_startButton;
    MenuButton * m_quitButton;

};
