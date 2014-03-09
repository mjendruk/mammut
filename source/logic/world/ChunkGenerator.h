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
    void ChunkGenerator::createOrdinaryLevel(CuboidChunk & chunk);
    void createWall(CuboidChunk & chunk, float distanceToNextThousand, bool createStripe);

protected:
    std::mt19937 m_generator;
    double m_zDistance;

};
