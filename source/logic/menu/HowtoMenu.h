#pragma once

#include "Menu.h"

class MenuButton;
class MenuText;

class HowtoMenu : public Menu
{
    Q_OBJECT

public:
    HowtoMenu();
    virtual ~HowtoMenu();

signals:
    void toMainMenuPressed();

protected:
    QList<MenuItem *> initializeMenuItems();
    void connectSignals();

protected:
    MenuButton * m_toMainMenuButton;

};
