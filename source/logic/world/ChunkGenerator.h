#pragma once

#include <random>

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
    void createWall(CuboidChunk & chunk, bool createStripe);

protected:
    std::mt19937 m_generator;
    glm::vec3 m_nextTranslation;
    double m_zDistance;

};
