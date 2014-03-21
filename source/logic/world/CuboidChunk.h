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
    
    const QList<Cuboid *> & cuboids() const;

protected:
    QList<Cuboid *> m_cuboids;
};
