#pragma once

#include "AbstractApplication.h"

class Game;

class Application : public AbstractApplication
{
    Q_OBJECT
public:
    Application(int & argc, char ** argv);
    virtual ~Application();

protected:
    Game * m_game;

};
