#include "Game.h"

#include <ratio>

#include <QTimer>
#include <QThread>
#include <QDebug>
#include <QMouseEvent>

#include <rendering/menu/MenuRenderer.h>
#include <rendering/world/GameWorldRenderer.h>

#include <rendering/menu/BlankBackground.h>
#include <rendering/menu/ScreenshotBackground.h>

#include <logic/world/GameMechanics.h>
#include <logic/menu/GameOverMenu.h>
#include <logic/menu/HighscoreMenu.h>
#include <logic/menu/MainMenu.h>
#include <logic/menu/PauseMenu.h>
#include <logic/menu/NewHighscoreMenu.h>

#include <sound/Sound.h>
#include <sound/SoundManager.h>

Game::Game(int & argc, char ** argv)
:   AbstractApplication(argc, argv)
,   m_loop(false)
,   m_paused(false)
{
    initializeWindow();
    initializeRenderers();
    
    m_highscoreList.addScore("Max", 5000);
    m_highscoreList.addScore("Johannes", 4000);
    m_highscoreList.addScore("Clemens", 3000);
    m_highscoreList.addScore("Caro", 2000);
    m_highscoreList.addScore("Friedrich", 1000);
    m_highscoreList.addScore("Moritz", 900);
    m_highscoreList.addScore("Karsten", 800);
    m_highscoreList.addScore("Yannick", 700);
    m_highscoreList.addScore("Aileen", 600);
    m_highscoreList.addScore("Noop", 500);

    
    showMainMenu();
    
    QTimer::singleShot(0, this, SLOT(run()));
}

Game::~Game()
{
}

void Game::run()
{
    m_loop = true;

    long double delta = 1 / 60.0f * std::nano::den;
    long double nextTime = m_timer.elapsed();
    
    while(m_loop) 
    {
        if (m_timer.elapsed() < nextTime)
        {
            unsigned long sleepTime =
            static_cast<unsigned long>((nextTime - m_timer.elapsed()) / std::nano::den *
                                                                        std::milli::den);
            
            if (sleepTime > 0)
                QThread::msleep(sleepTime);
        }
        
        SoundManager::instance().updateSoundSystem();
        QCoreApplication::processEvents();
        
        nextTime += delta;
        if (!m_paused)
            m_activeMechanics->update(delta / std::nano::den);
        
        if (m_timer.elapsed() < nextTime)
        {
            if(!m_window.isMinimized())
                m_canvas->render();
        }
    }
}

void Game::startGame()
{
    m_gameMechanics.reset(new GameMechanics());
    m_gameWorldRenderer->setGameMechanics(m_gameMechanics.get());
    
    connect(m_gameMechanics.get(), &GameMechanics::pause, this, &Game::pauseGame);
    connect(m_gameMechanics.get(), &GameMechanics::gameOver, this, &Game::endGame);
    
    m_activeMechanics = m_gameMechanics;
    m_canvas->setRenderer(m_gameWorldRenderer.get());
}

void Game::pauseGame()
{
    std::shared_ptr<PauseMenu> menu(new PauseMenu());
    
    connect(menu.get(), &PauseMenu::resumePressed, this, &Game::resumeGame);
    connect(menu.get(), &PauseMenu::toMainMenuPressed, this, &Game::showMainMenu);
    
    m_activeMechanics = menu;
    m_menuRenderer->setMenu(menu.get());
    
    m_canvas->setRenderer(m_gameWorldRenderer.get());
    m_screenshotBackground->setScreenshot(m_canvas->screenshot());
    m_menuRenderer->setBackground(m_screenshotBackground.get());
    
    m_canvas->setRenderer(m_menuRenderer.get());
}

void Game::resumeGame()
{
    m_activeMechanics = m_gameMechanics;
    m_canvas->setRenderer(m_gameWorldRenderer.get());
}

void Game::showMainMenu()
{
    std::shared_ptr<MainMenu> menu(new MainMenu());
    
    connect(menu.get(), &MainMenu::startPressed, this, &Game::startGame);
    connect(menu.get(), &MainMenu::highscorePressed, this, &Game::showHighscoreMenu);
    connect(menu.get(), &MainMenu::quitPressed, this, &Game::quit);
    
    m_activeMechanics = menu;
    m_menuRenderer->setMenu(menu.get());
    
    m_menuRenderer->setBackground(m_blankBackground.get());
    m_canvas->setRenderer(m_menuRenderer.get());
}

void Game::endGame(int score)
{
    if (m_highscoreList.isHighscore(score))
    {
        showNewHighscoreMenu(score);
    }
    else
    {
        showGameOverMenu(score);
    }
}

void Game::showHighscoreMenu()
{
    std::shared_ptr<HighscoreMenu> menu(new HighscoreMenu(m_highscoreList.scores()));
    
    connect(menu.get(), &HighscoreMenu::backPressed, this, &Game::showMainMenu);
    
    m_activeMechanics = menu;
    m_menuRenderer->setMenu(menu.get());
    
    m_menuRenderer->setBackground(m_blankBackground.get());
    m_canvas->setRenderer(m_menuRenderer.get());
}

void Game::showNewHighscoreMenu(int score)
{
    std::shared_ptr<NewHighscoreMenu> menu(new NewHighscoreMenu("Max", score));
    
    connect(menu.get(), &NewHighscoreMenu::nameEntered, [this, score](const QString & name)
            {
                m_highscoreList.addScore(name, score);
                showGameOverMenu(score);
            });
    
    m_activeMechanics = menu;
    m_menuRenderer->setMenu(menu.get());
    
    m_menuRenderer->setBackground(m_blankBackground.get());
    m_canvas->setRenderer(m_menuRenderer.get());
}

void Game::showGameOverMenu(int score)
{
    std::shared_ptr<GameOverMenu> menu(new GameOverMenu(score));
    
    connect(menu.get(), &GameOverMenu::retryPressed, this, &Game::startGame);
    connect(menu.get(), &GameOverMenu::toMainMenuPressed, this, &Game::showMainMenu);
    
    m_activeMechanics = menu;
    m_menuRenderer->setMenu(menu.get());
    
    m_menuRenderer->setBackground(m_blankBackground.get());    
    m_canvas->setRenderer(m_menuRenderer.get());
}

void Game::quit()
{
    m_loop = false;
    QApplication::quit();
}

void Game::initializeWindow()
{
    QSurfaceFormat format;
    format.setVersion(4, 1);
    format.setDepthBufferSize(24);
    format.setProfile(QSurfaceFormat::CoreProfile);

    m_canvas = new Canvas(format);
    m_canvas->installEventFilter(this);
    m_canvas->setSwapInterval(Canvas::NoVerticalSyncronization);
    
    QWidget * canvasWidget = QWidget::createWindowContainer(m_canvas);
    
    m_window.setCentralWidget(canvasWidget);
    m_window.setMinimumSize(800, 600);
    m_window.setFocusProxy(canvasWidget);
    m_window.setFocus();
    m_window.show();
}

void Game::initializeRenderers()
{
    m_menuRenderer.reset(new MenuRenderer());
    m_gameWorldRenderer.reset(new GameWorldRenderer());
    m_blankBackground.reset(new BlankBackground());
    m_screenshotBackground.reset(new ScreenshotBackground());
}

bool Game::eventFilter(QObject * obj, QEvent * event)
{
    switch (event->type())
    {
        case QEvent::KeyPress:
            keyPressed((QKeyEvent *)event);
            break;
            
        case QEvent::KeyRelease:
            keyReleased((QKeyEvent *)event);
            break;
            
        case QEvent::Hide:
            m_loop = false;
            break;
            
        default:
            return QObject::eventFilter(obj, event);
            break;
    }
    
    return true;
}


void Game::keyPressed(QKeyEvent * keyEvent)
{
    if (keyEvent->isAutoRepeat())
        return;
    
    if (keyEvent->key() == Qt::Key_Return && keyEvent->modifiers() == Qt::AltModifier)
    {
        if (m_window.isFullScreen())
            m_window.showNormal();
        else
            m_window.showFullScreen();
        
        return;
    }

    if (keyEvent->key() == Qt::Key_Space)
        m_paused = !m_paused;
    
    m_activeMechanics->keyPressed(keyEvent);
}

void Game::keyReleased(QKeyEvent * keyEvent)
{
    if (keyEvent->isAutoRepeat())
        return;

    m_activeMechanics->keyReleased(keyEvent);
}
