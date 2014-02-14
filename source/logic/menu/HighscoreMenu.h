#pragma once

#include <QList>

#include "Menu.h"

class MenuButton;
class HighscoreListItem;
class HighscoreEntry;

class HighscoreMenu : public Menu
{
    Q_OBJECT

public:
    HighscoreMenu(const QList<HighscoreEntry> & scores);
    virtual ~HighscoreMenu();

signals:
    void backPressed();

protected:
    QList<MenuItem *> initializeMenuItems(const QList<HighscoreEntry> & scores);
    void connectSignals();

protected:
    HighscoreListItem * m_highscore;
    MenuButton * m_backButton;

};
