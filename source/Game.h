#pragma once

#include "AbstractApplication.h"

class GameLogic;
class Renderer;

class Game : public AbstractApplication
{
    Q_OBJECT
public:
    Game(int & argc, char ** argv);
    virtual ~Game();

public slots:
    void run();

protected:
    GameLogic * m_gameLogic;
    Renderer * m_renderer;

    bool m_loop;

};
