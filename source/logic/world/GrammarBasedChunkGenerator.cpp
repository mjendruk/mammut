#include "GrammarBasedChunkGenerator.h"

#include <algorithm>
#include <cassert>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "Cuboid.h"

const float GrammarBasedChunkGenerator::s_minSmallSize = 5.f;
const float GrammarBasedChunkGenerator::s_minLargeSize = 10.f;

GrammarBasedChunkGenerator::GrammarBasedChunkGenerator(int seed, float chunkLength, int numChunks)
:   m_generator(seed)
,   m_layoutDistribution(0, int(Layout::count)-1)
,   m_rotationDistribution(0, int(Rotation::count) - 1)
,   smallSizeDistribution(8.f, 4.f)
,   largeSizeDistribution(20.f, 4.f)
,   smallShuffleDistribution(-2.f, 2.f)
,   largeShiftDistribution(10.f, 15.f)
,   m_chunkLength(chunkLength)
,   m_zDistance(0.f)
,   m_lastSLCombination({ Layout::count, Rotation::count })
{
    assert(chunkLength > 0);

    for (int i = 0; i < numChunks; ++i)
        m_chunkList << QSharedPointer<CuboidChunk>(new CuboidChunk());

    createStartChunk(*m_chunkList.first().data());

    for (int i = 1; i < numChunks; ++i) {
        createChunk(*m_chunkList.at(i).data());
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

void GrammarBasedChunkGenerator::createChunk(CuboidChunk & chunk)
{
    Layout layout = Layout(m_layoutDistribution(m_generator));
    Rotation rot = Rotation(m_rotationDistribution(m_generator));

    if (layout == m_lastSLCombination.layout && rot == m_lastSLCombination.rotation)
        rot = Rotation((int(rot) + 1 ) % int(Rotation::count));

    m_lastSLCombination = {layout, rot};

    switch (layout)
    {
    case Layout::single:
        createSingleChunk(chunk, rot);
        break;
    case Layout::parallel:
        createParallelChunk(chunk, rot);
        break;
    case Layout::displaced:
        createDisplacedChunk(chunk, rot);
        break;
    }

    m_zDistance += m_chunkLength;
}

void GrammarBasedChunkGenerator::createSingleChunk(CuboidChunk & chunk, Rotation rot)
{
    float xSize = std::max(s_minSmallSize, float(largeSizeDistribution(m_generator)));
    float ySize = std::max(s_minLargeSize, float(smallSizeDistribution(m_generator)));

    float xPos = smallShuffleDistribution(m_generator);
    float yPos = largeShiftDistribution(m_generator);

    glm::mat3 rotation = rotate(rot);

    chunk.add(new Cuboid(
        glm::abs(rotation * glm::vec3(xSize, ySize, 60.f)),
        rotation * glm::vec3(xPos, yPos, -30.f - m_zDistance)));
}

void GrammarBasedChunkGenerator::createParallelChunk(CuboidChunk & chunk, Rotation rot)
{
    float xSizeRight = std::max(s_minSmallSize, float(smallSizeDistribution(m_generator)));
    float ySizeRight = std::max(s_minLargeSize, float(largeSizeDistribution(m_generator)));

    float xSizeLeft = std::max(s_minSmallSize, float(smallSizeDistribution(m_generator)));
    float ySizeLeft = std::max(s_minLargeSize, float(largeSizeDistribution(m_generator)));

    float xPosRight = largeShiftDistribution(m_generator);
    float yPosRight = smallShuffleDistribution(m_generator);

    float xPosLeft = -largeShiftDistribution(m_generator);
    float yPosLeft = smallShuffleDistribution(m_generator);

    glm::mat3 rotation = rotate(rot);

    chunk.add(new Cuboid(
        glm::abs(rotation * glm::vec3(xSizeRight, ySizeRight, 60.f)),
        rotation * glm::vec3(xPosRight, yPosRight, -30.f - m_zDistance)));

    chunk.add(new Cuboid(
        glm::abs(rotation * glm::vec3(xSizeLeft, ySizeLeft, 60.f)),
        rotation * glm::vec3(xPosLeft, yPosLeft, -30.f - m_zDistance)));
}

void GrammarBasedChunkGenerator::createDisplacedChunk(CuboidChunk & chunk, Rotation rot)
{
    float xSizeTop = std::max(s_minLargeSize, float(largeSizeDistribution(m_generator)));
    float ySizeTop = std::max(s_minSmallSize, float(smallSizeDistribution(m_generator)));

    float xSizeLeft = std::max(s_minSmallSize, float(smallSizeDistribution(m_generator)));
    float ySizeLeft = std::max(s_minLargeSize, float(largeSizeDistribution(m_generator)));

    float xPosTop = smallShuffleDistribution(m_generator);
    float yPosTop = largeShiftDistribution(m_generator);

    float xPosLeft = -largeShiftDistribution(m_generator);
    float yPosLeft = smallShuffleDistribution(m_generator);

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
