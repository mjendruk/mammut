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
    void createOrdinaryLevel(CuboidChunk & chunk);
    void createWall(CuboidChunk & chunk, float distanceToNextThousand, bool createStripe);
    void createBoostDistribution();

protected:
    static const int s_chunksPerBoostDistribution;
    std::mt19937 m_generator;
    double m_zDistance;
    int m_boostDistribution[10];
    int m_numUsedBoostDistributions;
};
