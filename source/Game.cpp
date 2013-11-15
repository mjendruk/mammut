#include "Game.h"

#include "Application.h"
#include "Renderer.h"
#include "GameLogic.h"


Game::Game(Application * application)
:	m_application(application)
,   m_gameLogic(new GameLogic())
,   m_renderer(new Renderer(m_gameLogic))
,   m_loop(false)
{

}

Game::~Game()
{
	delete m_renderer;
	delete m_gameLogic;
}

void Game::run()
{
    m_loop = true;

    while(m_loop) 
	{
        m_application->processEvents();
        m_gameLogic->update(9001);
		m_renderer->render();
    }
}
