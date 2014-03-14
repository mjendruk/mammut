#pragma once

#include "Menu.h"

class MenuButton;

class PauseMenu : public Menu
{
    Q_OBJECT

public:
    PauseMenu();
    virtual ~PauseMenu();

signals:
    void resumePressed();
    void toMainMenuPressed();

protected:
    QList<MenuItem *> initializeMenuItems();
    void connectSignals();

protected:
    MenuButton * m_resumeButton;
    MenuButton * m_toMainMenuButton;

};
