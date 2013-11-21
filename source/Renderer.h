#pragma once

#include <QVector>
#include "Drawable.h"
#include "Painter.h"

class QObject;

class Canvas;
class GameLogic;
class Cuboid;

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

    const QVector<Cuboid *> * m_cuboids;
    Drawable m_cuboidDrawable;
};
