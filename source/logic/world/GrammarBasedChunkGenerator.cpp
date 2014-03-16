#include "GrammarBasedChunkGenerator.h"

#include <QDebug>

#include <glm/glm.hpp>

#include "Cuboid.h"


GrammarBasedChunkGenerator::GrammarBasedChunkGenerator(int seed, float chunkLength, int numChunks)
:   m_generator(seed)
,   m_chunkLength(chunkLength)
,   m_zDistance(0.f)
{
    for (int i = 0; i < numChunks; ++i) {
        QSharedPointer<CuboidChunk> chunk(new CuboidChunk);
        createStartChunk(*chunk.data());
        m_chunkList << chunk;
    }

}

GrammarBasedChunkGenerator::~GrammarBasedChunkGenerator()
{
    //clear Lists
}

bool GrammarBasedChunkGenerator::hasNextChunk()
{
    return !m_chunkList.isEmpty();
}

QSharedPointer<CuboidChunk> GrammarBasedChunkGenerator::nextChunk()
{
    return m_chunkList.takeFirst();
}

void GrammarBasedChunkGenerator::createStartChunk(CuboidChunk & chunk)
{
    chunk.add(new Cuboid(
        glm::vec3(20.f, 10.f, 80.f),
        glm::vec3(0.f, -2.f, -20.f)));

    m_zDistance += m_chunkLength;
}