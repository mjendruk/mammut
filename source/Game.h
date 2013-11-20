#pragma once


#include "AbstractApplication.h"
#include "GameLogic.h"
#include "Renderer.h"

class QMouseEvent;

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
    void mouseMoveEvent(QMouseEvent * event);
    bool eventFilter(QObject *obj, QEvent *event);

protected:
    GameLogic m_gameLogic;
    Renderer m_renderer;

    bool m_loop;
};
