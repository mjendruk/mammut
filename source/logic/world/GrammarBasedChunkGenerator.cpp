#include "GrammarBasedChunkGenerator.h"

#include <algorithm>

#include <QDebug>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "Cuboid.h"


GrammarBasedChunkGenerator::GrammarBasedChunkGenerator(int seed, float chunkLength, int numChunks)
:   m_generator(seed)
,   m_layoutDistribution(0, int(Layout::count)-1)
,   m_rotationDistribution(0, int(Rotation::count) - 1)
,   m_chunkLength(chunkLength)
,   m_zDistance(0.f)
,   m_lastSLCombination({ Layout::count, Rotation::count })
{
    assert(chunkLength > 0);

    //in init auslagern?

    for (int i = 0; i < numChunks; ++i)
        m_chunkList << QSharedPointer<CuboidChunk>(new CuboidChunk());

    createStartChunk(*m_chunkList.first().data());

    for (int i = 1; i < numChunks; ++i) {
        createSLChunk(*m_chunkList.at(i).data());
    }
}

GrammarBasedChunkGenerator::~GrammarBasedChunkGenerator()
{
    m_chunkList.clear();
}

bool GrammarBasedChunkGenerator::hasNextChunk()
{
    return !m_chunkList.isEmpty();
}

QSharedPointer<CuboidChunk> GrammarBasedChunkGenerator::nextChunk()
{
    assert(hasNextChunk());
    return m_chunkList.takeFirst();
}

void GrammarBasedChunkGenerator::createStartChunk(CuboidChunk & chunk)
{
    chunk.add(new Cuboid(
        glm::vec3(20.f, 10.f, 80.f),
        glm::vec3(0.f, -5.f, -20.f - m_zDistance)));

    m_zDistance += m_chunkLength;
}

void GrammarBasedChunkGenerator::createSLChunk(CuboidChunk & chunk)
{
    Layout layout = Layout(m_layoutDistribution(m_generator));
    Rotation rot = Rotation(m_rotationDistribution(m_generator));

    if (layout == m_lastSLCombination.layout && rot == m_lastSLCombination.rotation)
        rot = Rotation((int(rot) + 1 ) % int(Rotation::count));

    m_lastSLCombination = {layout, rot};

    switch (layout)
    {
    case Layout::single:
        createSLSingleChunk(chunk, rot);
        break;
    case Layout::parallel:
        createSLParallelChunk(chunk, rot);
        break;
    case Layout::displaced:
        createSLDisplacedChunk(chunk, rot);
        break;
    }

    m_zDistance += m_chunkLength;
}

void GrammarBasedChunkGenerator::createSLSingleChunk(CuboidChunk & chunk, Rotation rot)
{
    std::normal_distribution<> sizeXDistribution(10, 4);
    std::normal_distribution<> sizeYDistribution(20, 2);
    std::uniform_real_distribution<> positionXDistribution(-2.f, 2.f);
    std::uniform_real_distribution<> positionYDistribution(10.f, 15.f);

    float xSize = std::max(10.f, float(sizeXDistribution(m_generator)));
    float ySize = std::max(5.f, float(sizeYDistribution(m_generator)));

    float xPos = positionXDistribution(m_generator);
    float yPos = positionYDistribution(m_generator);

    glm::mat3 rotation = rotate(rot);

    chunk.add(new Cuboid(
        glm::abs(rotation * glm::vec3(xSize, ySize, 60.f)),
        rotation * glm::vec3(xPos, yPos, -30.f - m_zDistance)));
}

void GrammarBasedChunkGenerator::createSLParallelChunk(CuboidChunk & chunk, Rotation rot)
{
    std::normal_distribution<> sizeXDistribution(10, 4);
    std::normal_distribution<> sizeYDistribution(20, 2);
    std::uniform_real_distribution<> positionXDistribution(10.f, 15.f);
    std::uniform_real_distribution<> positionYDistribution(-2.f, 2.f);

    float xSizeRight = std::max(10.f, float(sizeXDistribution(m_generator)));
    float ySizeRight = std::max(5.f, float(sizeYDistribution(m_generator)));

    float xSizeLeft = std::max(10.f, float(sizeXDistribution(m_generator)));
    float ySizeLeft = std::max(5.f, float(sizeYDistribution(m_generator)));

    float xPosRight = positionXDistribution(m_generator);
    float yPosRight = positionYDistribution(m_generator);

    float xPosLeft = -positionXDistribution(m_generator);
    float yPosLeft = positionYDistribution(m_generator);

    glm::mat3 rotation = rotate(rot);

    chunk.add(new Cuboid(
        glm::abs(rotation * glm::vec3(xSizeRight, ySizeRight, 60.f)),
        rotation * glm::vec3(xPosRight, yPosRight, -30.f - m_zDistance)));

    chunk.add(new Cuboid(
        glm::abs(rotation * glm::vec3(xSizeLeft, ySizeLeft, 60.f)),
        rotation * glm::vec3(xPosLeft, yPosLeft, -30.f - m_zDistance)));
}

void GrammarBasedChunkGenerator::createSLDisplacedChunk(CuboidChunk & chunk, Rotation rot)
{
    std::normal_distribution<> sizeLargeDistribution(20, 2);
    std::normal_distribution<> sizeSmallDistribution(10, 4);

    std::uniform_real_distribution<> positionTopXDistribution(-2.f, 2.f);
    std::uniform_real_distribution<> positionTopYDistribution(10.f, 15.f);

    std::uniform_real_distribution<> positionLeftXDistribution(-15.f, -10.f);
    std::uniform_real_distribution<> positionLeftYDistribution(-2.f, 2.f);

    float xSizeTop = std::max(10.f, float(sizeLargeDistribution(m_generator)));
    float ySizeTop = std::max(5.f, float(sizeSmallDistribution(m_generator)));

    float xSizeLeft = std::max(5.f, float(sizeSmallDistribution(m_generator)));
    float ySizeLeft = std::max(10.f, float(sizeLargeDistribution(m_generator)));

    float xPosTop = positionTopXDistribution(m_generator);
    float yPosTop = positionTopYDistribution(m_generator);

    float xPosLeft = positionLeftXDistribution(m_generator);
    float yPosLeft = positionLeftYDistribution(m_generator);

    glm::mat3 rotation = rotate(rot);

    chunk.add(new Cuboid(
        glm::abs(rotation * glm::vec3(xSizeTop, ySizeTop, 60.f)),
        rotation * glm::vec3(xPosTop, yPosTop, -30.f - m_zDistance)));

    chunk.add(new Cuboid(
        glm::abs(rotation * glm::vec3(xSizeLeft, ySizeLeft, 60.f)),
        rotation * glm::vec3(xPosLeft, yPosLeft, -30.f - m_zDistance)));
}

glm::mat3 GrammarBasedChunkGenerator::rotate(Rotation rot)
{
    float angle = 0.f;
    switch (rot)
    {
    case Rotation::noRotation:
        break;
    case Rotation::rotate90:
        angle = 90.f;
        break;
    case Rotation::rotate180:
        angle = 180.f;
        break;
    case Rotation::rotate270:
        angle = 270.f;
        break;
    }

    const glm::mat3 rotation = glm::mat3(glm::rotate(angle, glm::vec3(0.0f, 0.0f, 1.0f)));
    return rotation;
}
