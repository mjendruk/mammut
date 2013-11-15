#pragma once

class Canvas;
class AbstractPainter;
class GameLogic;

class Renderer
{
public:
    Renderer(GameLogic * gameLogic);
    ~Renderer();

    void render();

protected:
	Canvas * m_canvas;
	AbstractPainter * m_painter;
	GameLogic * m_gameLogic;
};
