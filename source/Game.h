#pragma once

#include <QScopedPointer>
#include <QMainWindow>

#include <glow/Timer.h>

#include <game_mechanics/GameMechanics.h>
#include <game_world_rendering/Canvas.h>
#include <menu/MenuMechanics.h>

#include "AbstractApplication.h"

class QMouseEvent;

class Game : public AbstractApplication
{
    Q_OBJECT
    
public:
    Game(int & argc, char ** argv);
    virtual ~Game();

public slots:
    void run();
    void start();
    void quit();
    
protected:
    void mouseMoveEvent(QMouseEvent * event);
    bool eventFilter(QObject * obj, QEvent * event);
    void keyPressed(QKeyEvent * keyEvent);
    void keyReleased(QKeyEvent * keyEvent);

protected:
    GameMechanics m_gameMechanics;
    MenuMechanics m_menuMechanics;
    
    Mechanics * m_activeMechanics;
    
    glow::Timer m_timer;
    
    QMainWindow m_window;
    Canvas * m_canvas;

    bool m_loop;
    bool m_paused;
};
