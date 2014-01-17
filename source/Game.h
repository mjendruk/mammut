#pragma once

#include <QScopedPointer>
#include <QMainWindow>

#include <glow/Timer.h>

#include <game_world_rendering/Canvas.h>
#include <menu/MainMenu.h>
#include <menu/PauseMenu.h>
#include <menu/MenuRenderer.h>

#include "AbstractApplication.h"

class QMouseEvent;
class GameMechanics;
class GameWorldRenderer;

class Game : public AbstractApplication
{
    Q_OBJECT
    
public:
    Game(int & argc, char ** argv);
    virtual ~Game();

public slots:
    void run();
    void start();
    void pause();
    void resume();
    void toMainMenu();
    void quit();
    
protected:
    bool eventFilter(QObject * obj, QEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void keyPressed(QKeyEvent * keyEvent);
    void keyReleased(QKeyEvent * keyEvent);

protected:
    MainMenu m_mainMenu;
    PauseMenu m_pauseMenu;
    MenuRenderer m_menuRenderer;
    
    QScopedPointer<GameMechanics> m_gameMechanics;
    QScopedPointer<GameWorldRenderer> m_gameWorldRenderer;
    
    Mechanics * m_activeMechanics;
    
    glow::Timer m_timer;
    
    QMainWindow m_window;
    Canvas * m_canvas;

    bool m_loop;
    bool m_paused;
};
