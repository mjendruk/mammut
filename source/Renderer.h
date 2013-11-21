#pragma once

#include <QScopedPointer>
#include <QVector>

#include "Drawable.h"
#include "Painter.h"
#include "RenderCamera.h";

class QObject;

class Canvas;
class GameLogic;
class Cuboid;
class Mammoth;

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

    const QVector<Cuboid *> * m_cuboids;
    const Mammoth * m_mammoth;
    Drawable m_cuboidDrawable;
};
