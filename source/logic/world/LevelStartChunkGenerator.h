#pragma once

#include <random>

#include <QList>
#include <QMap>

#include <glm/glm.hpp>

#include "CuboidChunk.h"

class LevelStartChunkGenerator
{
public:
    LevelStartChunkGenerator(int seed, float chunkLength, int numChunks);
    ~LevelStartChunkGenerator();

    bool hasNextChunk();
    CuboidChunk * nextChunk();

protected: 
    enum class Layout { single, parallel, displaced, count };
    enum class Rotation{ noRotation, rotate90, rotate180, rotate270, count };

    struct chunkCombination
    {
        Layout layout;
        Rotation rotation;
    };

    static glm::mat3 rotate(Rotation rot);

    void createStartChunk(CuboidChunk & chunk);
    void createChunk(CuboidChunk & chunk);

    void createSingleChunk(CuboidChunk & chunk, Rotation rot);
    void createParallelChunk(CuboidChunk & chunk, Rotation rot);
    void createDisplacedChunk(CuboidChunk & chunk, Rotation rot);

protected:
    static const float s_minSmallSize;
    static const float s_minLargeSize;

    QList<CuboidChunk *> m_chunkList;

    std::mt19937 m_generator;
    std::uniform_int_distribution<> m_layoutDistribution;
    std::uniform_int_distribution<> m_rotationDistribution;

    std::normal_distribution<> smallSizeDistribution;
    std::normal_distribution<> largeSizeDistribution;
    std::uniform_real_distribution<> smallShuffleDistribution;
    std::uniform_real_distribution<> largeShiftDistribution;
  
    float m_chunkLength;
    float m_zDistance;
    float m_cuboidLength;

    chunkCombination m_lastCombination;
};
