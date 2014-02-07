#pragma once

#include "Menu.h"

class MenuButton;

class HighscoreMenu : public Menu
{
    Q_OBJECT

public:
    HighscoreMenu();
    virtual ~HighscoreMenu();

signals:
    void backPressed();

protected:
    QList<MenuItem *> initializeMenuItems();
    void connectSignals();

protected:
    MenuButton * m_backButton;

};
