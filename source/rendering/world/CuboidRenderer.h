#pragma once

#include <QList>

#include "CuboidDrawable.h"

class Cuboid;
class Painter;
class TessCuboidDrawable;

class CuboidRenderer
{
public:
    CuboidRenderer();
    ~CuboidRenderer();

    void draw(QList<Cuboid *> cuboids, Painter & painter);

protected:
    CuboidDrawable m_cuboidDrawable;
    QList<TessCuboidDrawable *> m_tessCuboidDrawables;
};
