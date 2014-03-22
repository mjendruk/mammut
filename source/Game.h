#pragma once

#include <memory>

#include <QWidget>

#include <glowutils/Timer.h>

#include <logic/highscore/HighscoreList.h>
#include "AbstractApplication.h"


class QMainWindow;

class Mechanics;
class GameMechanics;
class Menu;

class Canvas;
class MenuRenderer;
class GameWorldRenderer;

class AbstractBackground;
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

protected:
    void initializeWindow();
    void initializeRendering();
    
    void startGame();
    void pauseGame();
    void resumeGame();
    void endGame(int score);
    void showNewHighscoreMenu(int score);
    void showGameOverMenu(int score);
    void showMainMenu();
    void showHighscoreMenu();
    void showControlsMenu();
    void showCreditsMenu();
    void quit();
    
    void activateMenu(std::shared_ptr<Menu> menu, AbstractBackground * background);

    bool eventFilter(QObject * obj, QEvent * event) override;
    void keyPressed(QKeyEvent * keyEvent);
    void keyReleased(QKeyEvent * keyEvent);
    
    void makeWindowFullscreen();
    void makeWindowNormal();
    
protected:
    std::shared_ptr<GameMechanics> m_gameMechanics;
    std::shared_ptr<Mechanics> m_activeMechanics;
    
    std::unique_ptr<MenuRenderer> m_menuRenderer;
    std::unique_ptr<GameWorldRenderer> m_gameWorldRenderer;
    std::unique_ptr<BlankBackground> m_blankBackground;
    std::unique_ptr<ScreenshotBackground> m_screenshotBackground;
    
    HighscoreList m_highscoreList;
    
    Canvas * m_canvas;
    
    glowutils::Timer m_timer;
    bool m_loop;
    bool m_paused;

    QString m_lastName;
};
