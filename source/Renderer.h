#pragma once

#include <QObject>

#include "Drawable.h"
#include "Painter.h"
#include "RenderCamera.h"

class Canvas;
class GameLogic;

class Renderer
{
public:
    Renderer(GameLogic & gameLogic);
    ~Renderer();

    void render();

    void registerKeyHandler(QObject & keyHandler);

protected:
    void initialize();

protected:
    Canvas * m_canvas;
    Painter m_painter;
    GameLogic & m_gameLogic;
    bool m_initialized;

    RenderCamera m_camera;

    Drawable m_cuboidDrawable;
};
