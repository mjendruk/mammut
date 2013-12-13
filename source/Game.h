#pragma once

#include <glow/Timer.h>

#include "AbstractApplication.h"
#include "GameLogic.h"
#include "Renderer.h"

class QMouseEvent;


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
    bool eventFilter(QObject * obj, QEvent * event);
    void keyPressed(QKeyEvent * keyEvent);
    void keyReleased(QKeyEvent * keyEvent);

protected:
    GameLogic m_gameLogic;
    Renderer m_renderer;
    glow::Timer m_timer;

    bool m_loop;
    bool m_paused;
};
