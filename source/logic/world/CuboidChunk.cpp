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
}


void CuboidChunk::remove(int index)
{
    m_cuboids.removeAt(index);
}

const QList<Cuboid *> & CuboidChunk::cuboids() const
{
    return m_cuboids;
}
