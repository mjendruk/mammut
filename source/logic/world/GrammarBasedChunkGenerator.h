#pragma once

#include <random>

#include <QSharedPointer>
#include <QList>

#include <glm/glm.hpp>

#include "CuboidChunk.h"

class GrammarBasedChunkGenerator
{
public:
    GrammarBasedChunkGenerator(int seed, float chunkLength, int numChunks = 1);
    ~GrammarBasedChunkGenerator();

    bool hasNextChunk();
    QSharedPointer<CuboidChunk> nextChunk();

protected: 
    enum class Layout { parallel, displaced, count };
    enum class Rotation{ noRotation, rotate90, rotate180, rotate270, count };

    struct chunkCombination
    {
        Layout layout;
        Rotation rotation;
    };


    static glm::mat3 rotate(Rotation rot);
    void createStartChunk(CuboidChunk & chunk);
    void createSLChunk(CuboidChunk & chunk);
    void createDLChunk(CuboidChunk & chunk);

    void createSLParallelChunk(CuboidChunk & chunk, Rotation rot);
    void createSLDisplacedChunk(CuboidChunk & chunk, Rotation rot);

    void createDLParallelChunk(CuboidChunk & chunk, Rotation rot);
    void createDLDisplacedChunk(CuboidChunk & chunk, Rotation rot);

protected:
    QList<QSharedPointer<CuboidChunk>> m_chunkList;

    std::mt19937 m_generator;
    std::uniform_int_distribution<> m_layoutDistribution;
    std::uniform_int_distribution<> m_rotationDistribution;
  
    float m_chunkLength;
    float m_zDistance;

    chunkCombination m_lastSLCombination;
    chunkCombination m_lastDLCombination;
};
