#include "Game.h"

#include <ratio>

#include <QTimer>
#include <QThread>
#include <QDebug>
#include <QMouseEvent>

#include "glowutils/FileRegistry.h"

#include <logic/world/GameMechanics.h>
#include <rendering/menu/MenuRenderer.h>
#include <rendering/world/GameWorldRenderer.h>
#include <rendering/menu/BlankBackground.h>
#include <rendering/menu/ScreenshotBackground.h>
#include <sound/Sound.h>
#include <sound/SoundManager.h>

Game::Game(int & argc, char ** argv)
:   AbstractApplication(argc, argv)
,   m_loop(false)
,   m_paused(false)
{
    initializeWindow();
    initializeRenderers();
    connectSignals();
    
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
    
    connect(m_gameMechanics.get(), &GameMechanics::pause, this, &Game::showPauseMenu);
    connect(m_gameMechanics.get(), &GameMechanics::gameOver, this, &Game::showMainMenu);
    
    m_activeMechanics = m_gameMechanics.get();
    m_canvas->setRenderer(m_gameWorldRenderer.get());
}

void Game::resumeGame()
{
    m_activeMechanics = m_gameMechanics.get();
    m_canvas->setRenderer(m_gameWorldRenderer.get());
}

void Game::showPauseMenu()
{
    m_activeMechanics = &m_pauseMenu;
    m_canvas->setRenderer(m_gameWorldRenderer.get());
    m_screenshotBackground->setScreenshot(m_canvas->screenshot());
    m_menuRenderer->setMenu(&m_pauseMenu);
    m_menuRenderer->setBackground(m_screenshotBackground.get());
    m_canvas->setRenderer(m_menuRenderer.get());
}

void Game::showMainMenu()
{
    m_activeMechanics = &m_mainMenu;
    m_menuRenderer->setMenu(&m_mainMenu);
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

void Game::connectSignals()
{   
    connect(&m_mainMenu, &MainMenu::startPressed, this, &Game::startGame);
    connect(&m_mainMenu, &MainMenu::quitPressed, this, &Game::quit);
    
    connect(&m_pauseMenu, &PauseMenu::resumePressed, this, &Game::resumeGame);
    connect(&m_pauseMenu, &PauseMenu::toMainMenuPressed, this, &Game::showMainMenu);
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

    if (keyEvent->key() == Qt::Key_R)
    {
        glowutils::FileRegistry::instance().reloadAll();
        qDebug() << "reload shaders";
    }
    
    m_activeMechanics->keyPressed(keyEvent);
}

void Game::keyReleased(QKeyEvent * keyEvent)
{
    if (keyEvent->isAutoRepeat())
        return;

    m_activeMechanics->keyReleased(keyEvent);
}
