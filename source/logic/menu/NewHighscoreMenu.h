#pragma once

#include "Menu.h"


class QString;

class MenuInput;
class MenuText;

class NewHighscoreMenu : public Menu
{
    Q_OBJECT

public:
    NewHighscoreMenu(const QString & cachedName, int score);
    virtual ~NewHighscoreMenu();

public slots:
    void validateEnteredName(const QString & name);
    
signals:
    void nameEntered(const QString & name);

protected:
    QList<MenuItem *> initializeMenuItems(const QString & cachedName, int score);
    void connectSignals();

protected:
    MenuInput * m_input;

};
