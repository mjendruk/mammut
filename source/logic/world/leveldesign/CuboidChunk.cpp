#include "CuboidChunk.h"

#include <logic/world/Cuboid.h>

CuboidChunk::CuboidChunk()
{
}

CuboidChunk::~CuboidChunk()
{
}

void CuboidChunk::add(Cuboid * cuboid)
{
    m_cuboids << cuboid;
}

void CuboidChunk::remove(int index)
{
    delete m_cuboids[index];
    m_cuboids.removeAt(index);
}

QList<Cuboid *> & CuboidChunk::cuboids()
{
    return m_cuboids;
}
