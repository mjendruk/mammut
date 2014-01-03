
#pragma once

#include <QSharedPointer>
#include <glm/glm.hpp>

#include "CuboidChunk.h"

class btDiscreteDynamicsWorld;

class ChunkGenerator
{
public:
    ChunkGenerator(int seed, btDiscreteDynamicsWorld & dynamicsWorld);
    ~ChunkGenerator();

    QSharedPointer<CuboidChunk> nextChunk();

protected:
    int m_seed;
    glm::vec3 m_nextTranslation;
    
    btDiscreteDynamicsWorld & m_dynamicsWorld;
};
