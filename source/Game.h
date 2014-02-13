#pragma once

#include <memory>

#include <QMainWindow>

#include <glowutils/Timer.h>

#include <logic/menu/MainMenu.h>
#include <logic/menu/PauseMenu.h>
#include <rendering/Canvas.h>

#include "AbstractApplication.h"

class QMouseEvent;
class GameMechanics;

class MenuRenderer;
class GameWorldRenderer;
class BlankBackground;
class ScreenshotBackground;

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
    void initializeRenderers();
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
    
    std::unique_ptr<MenuRenderer> m_menuRenderer;
    std::unique_ptr<GameWorldRenderer> m_gameWorldRenderer;
    std::unique_ptr<BlankBackground> m_blankBackground;
    std::unique_ptr<ScreenshotBackground> m_screenshotBackground;
    
    QMainWindow m_window;
    Canvas * m_canvas;
    
    glowutils::Timer m_timer;
    bool m_loop;
    bool m_paused;
};
