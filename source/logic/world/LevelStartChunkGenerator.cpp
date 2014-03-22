#include "LevelStartChunkGenerator.h"

#include <algorithm>
#include <cassert>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "Cuboid.h"

const float LevelStartChunkGenerator::s_minSmallSize = 3.f;
const float LevelStartChunkGenerator::s_minLargeSize = 10.f;

LevelStartChunkGenerator::LevelStartChunkGenerator(int seed, float chunkLength, int numChunks)
:   m_generator(seed)
,   m_layoutDistribution(0, int(Layout::count)-1)
,   m_rotationDistribution(0, int(Rotation::count) - 1)
,   smallSizeDistribution(7.f, 4.f)
,   largeSizeDistribution(20.f, 4.f)
,   smallShuffleDistribution(-2.f, 2.f)
,   largeShiftDistribution(10.f, 15.f)
,   m_chunkLength(chunkLength)
,   m_zDistance(0.f)
,   m_cuboidLength(chunkLength - 10.f)
,   m_lastCombination({ Layout::count, Rotation::count })
{
    assert(chunkLength > 0);

    for (int i = 0; i < numChunks; ++i)
        m_chunkList << new CuboidChunk();

    createStartChunk(*m_chunkList.first());

    for (int i = 1; i < numChunks; ++i) {
        createChunk(*m_chunkList.at(i));
    }
}

LevelStartChunkGenerator::~LevelStartChunkGenerator()
{
    m_chunkList.clear();
}

bool LevelStartChunkGenerator::hasNextChunk()
{
    return !m_chunkList.isEmpty();
}

CuboidChunk * LevelStartChunkGenerator::nextChunk()
{
    assert(hasNextChunk());
    return m_chunkList.takeFirst();
}

void LevelStartChunkGenerator::createStartChunk(CuboidChunk & chunk)
{
    chunk.add(new Cuboid(
        glm::vec3(20.f, 10.f, 80.f),
        glm::vec3(0.f, -5.f, -20.f - m_zDistance)));

    m_zDistance += m_chunkLength;
}

void LevelStartChunkGenerator::createChunk(CuboidChunk & chunk)
{
    Layout layout = Layout(m_layoutDistribution(m_generator));
    Rotation rot = Rotation(m_rotationDistribution(m_generator));

    if (layout == m_lastCombination.layout && rot == m_lastCombination.rotation)
        rot = Rotation((int(rot) + 1 ) % int(Rotation::count));

    m_lastCombination = {layout, rot};

    switch (layout)
    {
    case Layout::single:
        //createSingleChunk(chunk, rot);
        //break;
    case Layout::parallel:
        createParallelChunk(chunk, rot);
        break;
    case Layout::displaced:
        createDisplacedChunk(chunk, rot);
        break;
    }

    m_zDistance += m_chunkLength;
}

void LevelStartChunkGenerator::createSingleChunk(CuboidChunk & chunk, Rotation rot)
{
    float xSize = std::max(s_minSmallSize, float(largeSizeDistribution(m_generator)));
    float ySize = std::max(s_minLargeSize, float(smallSizeDistribution(m_generator)));

    float xPos = smallShuffleDistribution(m_generator);
    float yPos = largeShiftDistribution(m_generator);

    float zShuffle = smallShuffleDistribution(m_generator);

    glm::mat3 rotation = rotate(rot);

    chunk.add(new Cuboid(
        glm::abs(rotation * glm::vec3(xSize, ySize, m_cuboidLength)),
        rotation * glm::vec3(xPos, yPos, -(m_cuboidLength / 2.f) - m_zDistance + zShuffle)));
}

void LevelStartChunkGenerator::createParallelChunk(CuboidChunk & chunk, Rotation rot)
{
    float xSizeRight = std::max(s_minSmallSize, float(smallSizeDistribution(m_generator)));
    float ySizeRight = std::max(s_minLargeSize, float(largeSizeDistribution(m_generator)));

    float xSizeLeft = std::max(s_minSmallSize, float(smallSizeDistribution(m_generator)));
    float ySizeLeft = std::max(s_minLargeSize, float(largeSizeDistribution(m_generator)));

    float xPosRight = largeShiftDistribution(m_generator);
    float yPosRight = smallShuffleDistribution(m_generator);

    float xPosLeft = -largeShiftDistribution(m_generator);
    float yPosLeft = smallShuffleDistribution(m_generator);

    float zShuffleLeft = smallShuffleDistribution(m_generator);
    float zShuffleRight = smallShuffleDistribution(m_generator);

    glm::mat3 rotation = rotate(rot);

    chunk.add(new Cuboid(
        glm::abs(rotation * glm::vec3(xSizeRight, ySizeRight, m_cuboidLength)),
        rotation * glm::vec3(xPosRight, yPosRight, -(m_cuboidLength / 2.f) - m_zDistance + zShuffleRight)));

    chunk.add(new Cuboid(
        glm::abs(rotation * glm::vec3(xSizeLeft, ySizeLeft, m_cuboidLength)),
        rotation * glm::vec3(xPosLeft, yPosLeft, -(m_cuboidLength / 2.f) - m_zDistance + zShuffleLeft)));
}

void LevelStartChunkGenerator::createDisplacedChunk(CuboidChunk & chunk, Rotation rot)
{
    float xSizeTop = std::max(s_minLargeSize, float(largeSizeDistribution(m_generator)));
    float ySizeTop = std::max(s_minSmallSize, float(smallSizeDistribution(m_generator)));

    float xSizeLeft = std::max(s_minSmallSize, float(smallSizeDistribution(m_generator)));
    float ySizeLeft = std::max(s_minLargeSize, float(largeSizeDistribution(m_generator)));

    float xPosTop = smallShuffleDistribution(m_generator);
    float yPosTop = largeShiftDistribution(m_generator);

    float xPosLeft = -largeShiftDistribution(m_generator);
    float yPosLeft = smallShuffleDistribution(m_generator);

    float zShuffleTop = smallShuffleDistribution(m_generator);
    float zShuffleLeft = smallShuffleDistribution(m_generator);

    glm::mat3 rotation = rotate(rot);

    chunk.add(new Cuboid(
        glm::abs(rotation * glm::vec3(xSizeTop, ySizeTop, m_cuboidLength)),
        rotation * glm::vec3(xPosTop, yPosTop, -(m_cuboidLength / 2.f) - m_zDistance + zShuffleTop)));

    chunk.add(new Cuboid(
        glm::abs(rotation * glm::vec3(xSizeLeft, ySizeLeft, m_cuboidLength)),
        rotation * glm::vec3(xPosLeft, yPosLeft, -(m_cuboidLength / 2.f) - m_zDistance + zShuffleLeft)));
}

glm::mat3 LevelStartChunkGenerator::rotate(Rotation rot)
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
