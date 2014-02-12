#pragma once

#include "Menu.h"


class QString;

class MenuButton;
class MenuInput;
class MenuText;

class NewHighscoreMenu : public Menu
{
    Q_OBJECT

public:
    NewHighscoreMenu(const QString & cachedName, int score);
    virtual ~NewHighscoreMenu();

    void onOkPressed();

signals:
    void nameEntered(const QString & name);

protected:
    QList<MenuItem *> initializeMenuItems(const QString & cachedName, int score);
    void connectSignals();

protected:
    MenuInput * m_input;
    MenuButton * m_okButton;

};
