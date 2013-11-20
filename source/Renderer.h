#pragma once

#include "Painter.h"

class Canvas;
class GameLogic;
class KeyHandler;
class QObject;

class Renderer
{
public:
    Renderer(GameLogic & gameLogic);
    ~Renderer();

    void render();

    void registerKeyHandler(QObject & keyHandler);

protected:
    Canvas * m_canvas;
    Painter m_painter;
    GameLogic & m_gameLogic;
};
