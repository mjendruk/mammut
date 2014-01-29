#pragma once

#include <memory>

#include <QMainWindow>

#include <glow/Timer.h>

#include <logic/menu/MainMenu.h>
#include <logic/menu/PauseMenu.h>
#include <rendering/Canvas.h>
#include <rendering/menu/MenuRenderer.h>
#include <rendering/world/GameWorldRenderer.h>

#include "AbstractApplication.h"

class QMouseEvent;
class GameMechanics;

class Game : public AbstractApplication
{
    Q_OBJECT
    
public:
    Game(int & argc, char ** argv);
    virtual ~Game();

public slots:
    void run();
    void startGame();
    void resumeGame();
    void showPauseMenu();
    void showMainMenu();
    void quit();
    
protected:
    void initializeWindow();
    void connectSignals();

    bool eventFilter(QObject * obj, QEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void keyPressed(QKeyEvent * keyEvent);
    void keyReleased(QKeyEvent * keyEvent);

protected:
    MainMenu m_mainMenu;
    PauseMenu m_pauseMenu;

    std::unique_ptr<GameMechanics> m_gameMechanics;
    
    Mechanics * m_activeMechanics;
    
    MenuRenderer m_menuRenderer;
    GameWorldRenderer m_gameWorldRenderer;
    
    QMainWindow m_window;
    Canvas * m_canvas;
    
    glow::Timer m_timer;
    bool m_loop;
    bool m_paused;
};
