#include "Game.h"

#include <ratio>

#include <QTimer>
#include <QThread>
#include <QDebug>
#include <QMouseEvent>

#include <game_mechanics/GameMechanics.h>
#include <game_world_rendering/GameWorldRenderer.h>

Game::Game(int & argc, char ** argv)
:   AbstractApplication(argc, argv)
,   m_menuRenderer(&m_mainMenu)
,   m_loop(false)
,   m_paused(false)
{
    QSurfaceFormat format;
    format.setVersion(4, 1);
    format.setDepthBufferSize(24);
    format.setProfile(QSurfaceFormat::CoreProfile);
    
    m_activeMechanics = &m_mainMenu;
    
    connect(&m_mainMenu, &MainMenu::startPressed, this, &Game::start);
    connect(&m_mainMenu, &MainMenu::quitPressed, this, &Game::quit);
    
    connect(&m_pauseMenu, &PauseMenu::resumePressed, this, &Game::resume);
    connect(&m_pauseMenu, &PauseMenu::toMainMenuPressed, this, &Game::toMainMenu);

    m_canvas = new Canvas(format, &m_menuRenderer);
    m_canvas->installEventFilter(this);
    m_canvas->setSwapInterval(Canvas::NoVerticalSyncronization);
    
    QWidget * canvasWidget = QWidget::createWindowContainer(m_canvas);
    
    m_window.setCentralWidget(canvasWidget);
    m_window.setMinimumSize(800, 600);
    m_window.setFocusProxy(canvasWidget);
    m_window.setFocus();
    m_window.show();
    
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
        QCoreApplication::processEvents();
        
        if (m_timer.elapsed() < nextTime)
        {
            unsigned long sleepTime =
            static_cast<unsigned long>((nextTime - m_timer.elapsed()) / std::nano::den *
                                                                        std::milli::den);
            
            if (sleepTime > 0)
                QThread::msleep(sleepTime);
        }
        
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

void Game::start()
{
    auto gameMechanics = new GameMechanics();
    auto gameWorldRenderer = new GameWorldRenderer(*gameMechanics);
    
    m_gameMechanics.reset(gameMechanics);
    m_gameWorldRenderer.reset(gameWorldRenderer);
    
    connect(m_gameMechanics.data(), &GameMechanics::pausePressed, this, &Game::pause);
    
    m_activeMechanics = m_gameMechanics.data();
    m_canvas->changeRenderer(m_gameWorldRenderer.data());
}

void Game::pause()
{
    m_activeMechanics = &m_pauseMenu;
    m_menuRenderer.setMenu(&m_pauseMenu);
    m_canvas->changeRenderer(&m_menuRenderer);
}

void Game::resume()
{
    m_activeMechanics = m_gameMechanics.data();
    m_canvas->changeRenderer(m_gameWorldRenderer.data());
}

void Game::toMainMenu()
{
//    m_activeMechanics = &m_mainMenu;
//    m_menuRenderer.setMenu(&m_mainMenu);
}

void Game::quit()
{
    m_loop = false;
    QApplication::quit();
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
