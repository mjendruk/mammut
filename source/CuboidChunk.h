
#pragma once

#include <glowutils/AxisAlignedBoundingBox.h>

#include "Cuboid.h"

class CuboidChunk
{
public:
    CuboidChunk();
    ~CuboidChunk();

    void add(Cuboid * cuboid);
    
    const QList<Cuboid *> & cuboids() const;
    const glow::AxisAlignedBoundingBox boundingBox() const;

protected:
    QList<Cuboid *> m_cuboids;
    glow::AxisAlignedBoundingBox m_boundingBox;

};
