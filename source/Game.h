#pragma once

#include <QObject>

class Application;
class GameLogic;
class Renderer;

class Game : public QObject
{
	Q_OBJECT
public:
    Game(Application * application);
    ~Game();

public slots:
	void run();

protected:
    Application * m_application;
    GameLogic * m_gameLogic;
    Renderer * m_renderer;

    bool m_loop;

};
