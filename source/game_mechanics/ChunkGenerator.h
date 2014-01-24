#pragma once

#include <QSharedPointer>
#include <glm/glm.hpp>

#include "CuboidChunk.h"

class btDiscreteDynamicsWorld;

class ChunkGenerator
{
public:
    ChunkGenerator(int seed);
    ~ChunkGenerator();

    QSharedPointer<CuboidChunk> nextChunk();

protected:
    int m_seed;
    glm::vec3 m_nextTranslation;

};
