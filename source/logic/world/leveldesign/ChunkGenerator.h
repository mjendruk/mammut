#pragma once

#include <random>

#include <QStringList>
#include <QList>

#include "LevelStartChunkGenerator.h"

class CuboidChunk;
class Cuboid;

class ChunkGenerator
{
public:
    ChunkGenerator(long long seed = 2156);

    QList<Cuboid *> nextChunk();

    void addZShift(float zShift);

protected: 
    void createOrdinaryLevel(CuboidChunk & chunk);
    void createRawChunk(CuboidChunk & chunk, int numCouboids);
    void removeOverlaps(CuboidChunk & chunk, int maxNumOverlaps);
    void distributeBoosts(CuboidChunk & chunk);

    void createWall(CuboidChunk & chunk, float distanceToNextWall, bool createStripe);
    void createBoostDistribution();

    void printDebugStream();

protected:
    static const int s_numGrammarChunks;

    static const float s_chunkLength;
    static const float s_startIncreasingDifficulty;
    static const float s_stopIncreasingDifficulty;

    static const float s_minCuboidXYSize;
    static const float s_minCuboidOverlapSize;

    static const int s_wallStep;
    static const float s_wallSize;
    static const float s_wallThickness;

    static const int s_chunksPerBoostDistribution;
    static const int s_maxBoostsPerChunk;

    static const bool s_printDebug;

    LevelStartChunkGenerator m_levelStartChunkGenerator;

    std::mt19937 m_generator;
    float m_zDistance;
    float m_zPosition;
    int m_boostDistribution[10];
    int m_numUsedBoostDistributions;

    QStringList m_debugStream;
};
