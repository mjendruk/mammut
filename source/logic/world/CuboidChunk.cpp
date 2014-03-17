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

QList<Cuboid *> & CuboidChunk::cuboids()
{
    return m_cuboids;
}
