#pragma once

#include <QObject>

class GameLogic;
class Renderer;

class Game : public QObject
{
	Q_OBJECT
public:
    Game();
    ~Game();

public slots:
	void run();

protected:
    GameLogic * m_gameLogic;
    Renderer * m_renderer;

    bool m_loop;

};
