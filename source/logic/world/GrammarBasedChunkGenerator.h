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
    enum class Layout { parallel, displaced };
    enum class Rotation{ noRotation, rotate90, rotate180, rotate270, count };
    enum class Length{ sameLength, differenLength, count };

    struct chunkCombination
    {
        Layout layout;
        Rotation rotation;
        Length length;
    };
    // create A, B
    void createStartChunk(CuboidChunk & chunk);
    void createChunk(CuboidChunk & chunk, chunkCombination);

protected:
    QList<chunkCombination> m_chunkCombination;
    QList<QSharedPointer<CuboidChunk>> m_chunkList;

    std::mt19937 m_generator;
  
    float m_chunkLength;
    float m_zDistance;
};
