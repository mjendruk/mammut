#pragma once

#include <random>
#include <chrono>

#include <QSharedPointer>
#include <glm/glm.hpp>

#include "GrammarBasedChunkGenerator.h"

class CuboidChunk;

class ChunkGenerator
{
public:
    ChunkGenerator(long long seed = 2156);

    QSharedPointer<CuboidChunk> nextChunk();

protected: 
    void createOrdinaryLevel(CuboidChunk & chunk);
    void createRawChunk(CuboidChunk & chunk, int numCouboids);
    void removeOverlaps(CuboidChunk & chunk, int maxNumOverlaps);

    void createWall(CuboidChunk & chunk, float distanceToNextThousand, bool createStripe);

protected:
    static const int s_numGrammarChunks;

    static const float s_chunkLength;
    static const double s_startIncreasingSeverity;
    static const double s_stopIncreasingSeverity;

    static const float s_minCuboidOverlapSize;

    static const int s_wallStep;
    static const float s_wallSize;
    static const float s_wallThickness;

    GrammarBasedChunkGenerator m_grammarChunkGenerator;

    std::mt19937 m_generator;
    double m_zDistance;
};
