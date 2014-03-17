#pragma once

#include <QList>
#include <glowutils/AxisAlignedBoundingBox.h>

class Cuboid;

class CuboidChunk
{
public:
    CuboidChunk();
    ~CuboidChunk();

    void add(Cuboid * cuboid);
    void remove(int index);
    
    QList<Cuboid *> & cuboids();

protected:
    QList<Cuboid *> m_cuboids;
};
