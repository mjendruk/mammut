#include "CuboidChunk.h"

#include "Cuboid.h"

CuboidChunk::CuboidChunk()
{
}

CuboidChunk::~CuboidChunk()
{
    qDeleteAll(m_cuboids);
}

void CuboidChunk::add(Cuboid * cuboid)
{
    m_cuboids << cuboid;
    m_boundingBox.extend(cuboid->boundingBox().llf());
    m_boundingBox.extend(cuboid->boundingBox().urb());
}


void CuboidChunk::remove(int index)
{
    m_cuboids.removeAt(index);
}

const QList<Cuboid *> & CuboidChunk::cuboids() const
{
    return m_cuboids;
}

const glowutils::AxisAlignedBoundingBox CuboidChunk::boundingBox() const
{
    return m_boundingBox;
}
