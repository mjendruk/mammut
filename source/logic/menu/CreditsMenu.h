#pragma once

#include "Menu.h"

class MenuButton;
class MenuText;

class CreditsMenu : public Menu
{
    Q_OBJECT

public:
    CreditsMenu();
    virtual ~CreditsMenu();

signals:
    void toMainMenuPressed();

protected:
    QList<MenuItem *> initializeMenuItems();
    void connectSignals();

protected:
    MenuButton * m_toMainMenuButton;

};
