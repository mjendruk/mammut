#include "Game.h"

#include <ratio>

#include <QTimer>
#include <QThread>
#include <QDebug>
#include <QMouseEvent>

#include "rendering/Renderer.h"
#include "gamelogic/GameLogic.h"

Game::Game(int & argc, char ** argv)
: AbstractApplication(argc, argv)
,   m_gameLogic()
,   m_renderer(m_gameLogic)
,   m_loop(false)
,   m_paused(false)
{
    m_renderer.registerKeyHandler(*this);
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
            m_gameLogic.update(delta / std::nano::den);
        
        if (m_timer.elapsed() < nextTime)
            m_renderer.render();
    }
}

bool Game::eventFilter(QObject *obj, QEvent *event)
{
    switch (event->type())
    {
        case QEvent::KeyPress:
            keyPressed((QKeyEvent*)event);
            break;
            
        case QEvent::KeyRelease:
            keyReleased((QKeyEvent*)event);
            break;
            
        default:
            return QObject::eventFilter(obj, event);
            break;
    }
    
    return true;
}


void Game::keyPressed(QKeyEvent *keyEvent)
{
    if (keyEvent->isAutoRepeat())
        return;

    if (keyEvent->key() == Qt::Key_Space)
        m_paused = !m_paused;
    
    m_gameLogic.keyPressed(keyEvent->key());
}

void Game::keyReleased(QKeyEvent *keyEvent)
{
    if (keyEvent->isAutoRepeat())
        return;

    m_gameLogic.keyReleased(keyEvent->key());
}