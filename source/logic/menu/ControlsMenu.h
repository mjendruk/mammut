#pragma once

#include "Menu.h"

class MenuButton;
class MenuText;

class ControlsMenu : public Menu
{
    Q_OBJECT

public:
    ControlsMenu();
    virtual ~ControlsMenu();

signals:
    void toMainMenuPressed();

protected:
    QList<MenuItem *> initializeMenuItems();
    void connectSignals();

protected:
    MenuButton * m_toMainMenuButton;

};
