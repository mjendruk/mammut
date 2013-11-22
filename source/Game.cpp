#include "Game.h"

#include <QCoreApplication>
#include <QTimer>
#include <QMouseEvent>

#include "Renderer.h"
#include "GameLogic.h"

Game::Game(int & argc, char ** argv)
: AbstractApplication(argc, argv)
,   m_gameLogic()
,   m_renderer(m_gameLogic)
,   m_loop(false)
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

    while(m_loop) 
    {
        QCoreApplication::processEvents();
        m_gameLogic.update(16);
        m_renderer.render();
    }
}

bool Game::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        if (!((QKeyEvent*)event)->isAutoRepeat())
        {   
            m_gameLogic.keyPressed(((QKeyEvent*)event)->key());
        }
        
    }
    if (event->type() == QEvent::KeyRelease) {
        if (!((QKeyEvent*)event)->isAutoRepeat())
        {
            m_gameLogic.keyReleased(((QKeyEvent*)event)->key());
        }
    }
    else {
        // standard event processing
        return QObject::eventFilter(obj, event);
    }
}
