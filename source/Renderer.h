#pragma once

class Canvas;
class Painter;
class GameLogic;

class Renderer
{
public:
    Renderer(GameLogic * gameLogic);
    ~Renderer();

    void render();

protected:
	Canvas * m_canvas;
	Painter * m_painter;
	GameLogic * m_gameLogic;
};
