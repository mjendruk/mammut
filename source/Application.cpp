#include "Application.h"

#include <QTimer>

#include "Game.h"

Application::Application(
    int & argc
,   char ** argv)
:   AbstractApplication(argc, argv)
,   m_game(new Game(this))
{
    QTimer::singleShot(0, m_game, SLOT(run()));
}

Application::~Application()
{
    delete m_game;
}

