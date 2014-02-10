#pragma once

#include <memory>

#include <QMainWindow>

#include <glow/Timer.h>

#include <rendering/Canvas.h>
#include <logic/highscore/HighscoreList.h>
#include "AbstractApplication.h"

class QMouseEvent;

class GameMechanics;
class Mechanics;

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
    void showHighscore();
    void quit();
    
protected:
    void initializeWindow();
    void initializeRenderers();

    bool eventFilter(QObject * obj, QEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void keyPressed(QKeyEvent * keyEvent);
    void keyReleased(QKeyEvent * keyEvent);

protected:
    std::shared_ptr<GameMechanics> m_gameMechanics;
    std::shared_ptr<Mechanics> m_activeMechanics;
    
    std::unique_ptr<MenuRenderer> m_menuRenderer;
    std::unique_ptr<GameWorldRenderer> m_gameWorldRenderer;
    std::unique_ptr<BlankBackground> m_blankBackground;
    std::unique_ptr<ScreenshotBackground> m_screenshotBackground;
    
    HighscoreList m_highscoreList;
    
    QMainWindow m_window;
    Canvas * m_canvas;
    
    glow::Timer m_timer;
    bool m_loop;
    bool m_paused;
};
