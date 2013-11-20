#include "Game.h"

#include <QDebug>
#include <QCoreApplication>
#include <QTimer>
#include <QMouseEvent>

#include "Renderer.h"
#include "GameLogic.h"

Game::Game(int & argc, char ** argv)
: AbstractApplication(argc, argv)
,   m_gameLogic(GameLogic())
,   m_renderer(Renderer(m_gameLogic))
,   m_loop(false)
{
    m_renderer.registerKeyHandler(m_gameLogic.keyHandler());
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
        m_gameLogic.update(9001);
        m_renderer.render();
    }
}


void Game::mouseMoveEvent(QMouseEvent * event)
{
    qDebug() << "hi";
}
