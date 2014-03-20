#include "ChunkGenerator.h"

#include <algorithm>
#include <cassert>

#include <QDebug>

#include <glm/glm.hpp>

#include "Cuboid.h"

const int ChunkGenerator::s_numGrammarChunks = 5;

const float ChunkGenerator::s_chunkLength = 70.f;
const double ChunkGenerator::s_startIncreasingSeverity = 500.0;
const double ChunkGenerator::s_stopIncreasingSeverity = 3500.0;

const float ChunkGenerator::s_minCuboidOverlapSize = 1.f;

const int ChunkGenerator::s_wallStep = 1000;
const float ChunkGenerator::s_wallSize = 500.f;
const float ChunkGenerator::s_wallThickness = 5.f;

const int ChunkGenerator::s_chunksPerBoostDistribution = 10;
const int ChunkGenerator::s_maxBoostsPerChunk = 2;

ChunkGenerator::ChunkGenerator(long long seed)
:   m_grammarChunkGenerator(seed, s_chunkLength, s_numGrammarChunks)
,   m_generator(seed)
,   m_zDistance(0.0)
,   m_numUsedBoostDistributions(0)
{
    createBoostDistribution();
}

QSharedPointer<CuboidChunk> ChunkGenerator::nextChunk()
{
    // recalculate boostDistribution if necessary
    if (m_numUsedBoostDistributions == s_chunksPerBoostDistribution - 1)
        createBoostDistribution();

    if (m_grammarChunkGenerator.hasNextChunk()) {

        QSharedPointer<CuboidChunk> chunk = m_grammarChunkGenerator.nextChunk();
        distributeBoosts(*chunk.data());
        m_zDistance += s_chunkLength;

        return chunk;
    }

    QSharedPointer<CuboidChunk> chunk(new CuboidChunk);

    float distanceToNextThousand = s_wallStep - int(m_zDistance) % s_wallStep;

    if (distanceToNextThousand <= s_chunkLength) {
        createWall(
            *chunk.data(), 
            distanceToNextThousand, 
            m_zDistance > 2.f * s_wallStep ? false : true);
        return chunk;
    }

    createOrdinaryLevel(*chunk.data());
    return chunk;
}

void ChunkGenerator::createOrdinaryLevel(CuboidChunk & chunk)
{
    int numCuboids =     glm::smoothstep(s_startIncreasingSeverity, s_stopIncreasingSeverity, m_zDistance) * 46.0 + 4.0; // [4, 50]
    int maxNumOverlaps = glm::smoothstep(s_startIncreasingSeverity, s_stopIncreasingSeverity, m_zDistance) * 18.0; //[0, 18]

    qDebug() << "----------------------";
    qDebug() << "num Cuboids: " << numCuboids << "   num of max. allowed overlaps: " << maxNumOverlaps;

    createRawChunk(chunk, numCuboids);

    removeOverlaps(chunk, maxNumOverlaps);

    distributeBoosts(chunk);

    m_zDistance += s_chunkLength;
}

void ChunkGenerator::createRawChunk(CuboidChunk & chunk, int numCuboids)
{
    float xyDistribution =       glm::smoothstep(s_startIncreasingSeverity, s_stopIncreasingSeverity, m_zDistance) * 15 + 20; // [20, 35]
    float minSizeZDistribution = glm::smoothstep(s_startIncreasingSeverity, s_stopIncreasingSeverity, m_zDistance) * 20.0; // [0, 20]

    std::normal_distribution<> sizeXYDistribution(10, 4);
    std::uniform_real_distribution<> sizeZDistribution(50.0f - minSizeZDistribution, 60.0f);
    std::uniform_real_distribution<> positionXYDistribution(-xyDistribution, xyDistribution);
    std::uniform_real_distribution<> positionZDistribution(0.0f, 30.0f);

    for (int i = 0; i < numCuboids; ++i)
    {
        float xSize = std::max(float(sizeXYDistribution(m_generator)), 2.0f);
        float ySize = std::max(float(sizeXYDistribution(m_generator)), 2.0f);

        const glm::vec3 size(
            xSize,
            ySize,
            sizeZDistribution(m_generator));

        const glm::vec3 position(
            positionXYDistribution(m_generator),
            positionXYDistribution(m_generator),
            -positionZDistribution(m_generator));

        chunk.add(new Cuboid(size, position - glm::vec3(0.f, 0.f, m_zDistance)));
    }
}

void ChunkGenerator::removeOverlaps(CuboidChunk & chunk, int maxNumOverlaps)
{
    int numOverlaps = 0;
    int numDeletions = 0;

    QList<Cuboid *> cuboidList = chunk.cuboids();
    QList<int> removeIndexList;

    for (int i = 0; i < cuboidList.size() - 1; i++)
    for (int j = i + 1; j < cuboidList.size(); j++)
    {
        glm::vec2 c1URB = cuboidList.at(i)->boundingBox().urb().xy;
        glm::vec2 c1LLF = cuboidList.at(i)->boundingBox().llf().xy;

        glm::vec2 c2URB = cuboidList.at(j)->boundingBox().urb().xy;
        glm::vec2 c2LLF = cuboidList.at(j)->boundingBox().llf().xy;

        bool overlapped = !(c1LLF.x > c2URB.x || c2LLF.x > c1URB.x || c2LLF.y > c1URB.y || c1LLF.y > c2URB.y);

        if (overlapped) {
            ++numOverlaps;

            float minDeltaX = std::min(glm::abs(c1URB.x - c2URB.x), glm::abs(c1LLF.x - c2LLF.x));
            float minDeltaY = std::min(glm::abs(c1URB.y - c2URB.y), glm::abs(c1LLF.y - c2LLF.y));

            if ((minDeltaX < s_minCuboidOverlapSize || minDeltaY < s_minCuboidOverlapSize) && !(numOverlaps > maxNumOverlaps))
                qDebug() << "   " << "too small overlap -> delete cuboid";
            if ((numOverlaps > maxNumOverlaps) && !(minDeltaX < s_minCuboidOverlapSize || minDeltaY < s_minCuboidOverlapSize))
                qDebug() << "   " << "too many overlaps -> delete cuboid";
            if ((numOverlaps > maxNumOverlaps) && (minDeltaX < s_minCuboidOverlapSize || minDeltaY < s_minCuboidOverlapSize))
                qDebug() << "   " << "too many overlaps & too small overlap -> delete cuboid";

            if ((minDeltaX < s_minCuboidOverlapSize) || (minDeltaY < s_minCuboidOverlapSize) || (numOverlaps > maxNumOverlaps)) {
                removeIndexList << i;
                ++numDeletions;
                break;
            }
            else
                qDebug() << "   " << "overlap but no deletion";
        }
    }

    for (int i = removeIndexList.size() - 1; i >= 0; --i)
        chunk.remove(removeIndexList.at(i));

    qDebug() << "num overlaps" << numOverlaps;
    qDebug() << "num deletions" << numDeletions;
}

void ChunkGenerator::distributeBoosts(CuboidChunk & chunk)
{
    assert(m_boostDistribution[m_numUsedBoostDistributions] <= 3);

    int halfNumCuboids = chunk.cuboids().size() / 2;

    int numBoosts = std::min(m_boostDistribution[m_numUsedBoostDistributions], halfNumCuboids);

    int step = numBoosts > 0 ? chunk.cuboids().size() / numBoosts : 1;

    for (int i = 0; i < numBoosts; i += step)
        chunk.cuboids().at(i)->setBoost();

    ++m_numUsedBoostDistributions;

    qDebug() << "num boosts " << numBoosts;
}

void ChunkGenerator::createWall(CuboidChunk & chunk, float distanceToNextThousand, bool createStripe)
{
    const float zPosition = -(m_zDistance + distanceToNextThousand + s_wallThickness / 2.f + 1);
    const float minStripeSize = 5.f;

    std::uniform_real_distribution<> offsetDistribution(-30.0f, 30.0f);
    std::normal_distribution<> sizeDistribution(15.0f, 4.0f);

    float sizeX = std::max(minStripeSize, float(sizeDistribution(m_generator)));
    float sizeY = std::max(minStripeSize, float(sizeDistribution(m_generator)));

    float offsetX = float(offsetDistribution(m_generator));
    float offsetY = float(offsetDistribution(m_generator));

    chunk.add(new Cuboid(
        glm::vec3(s_wallSize, s_wallSize, s_wallThickness), 
        glm::vec3(-(s_wallSize + sizeX) / 2.f + offsetX, offsetY, zPosition)));

    chunk.add(new Cuboid(
        glm::vec3(s_wallSize, s_wallSize, s_wallThickness), 
        glm::vec3(+(s_wallSize + sizeX) / 2.f + offsetX, offsetY, zPosition)));

    if (!createStripe)
    {
        chunk.add(new Cuboid(
            glm::vec3(sizeX, s_wallSize, s_wallThickness), 
            glm::vec3(offsetX, -(s_wallSize + sizeY) / 2.f + offsetY, zPosition)));

        chunk.add(new Cuboid(
            glm::vec3(sizeX, s_wallSize, s_wallThickness), 
            glm::vec3(offsetX, +(s_wallSize + sizeY) / 2.f + offsetY, zPosition)));
    }

    qDebug() << "----------------------";
    qDebug() << "wall " << (createStripe ? "(stripe)" : "(no stripe)");
    qDebug() << "size: " << sizeX << " x " << sizeY;
    qDebug() << "offset: " << offsetX << " x " << offsetY;
    qDebug() << "distance: " << m_zDistance + distanceToNextThousand << "     distance to last chunk: " << distanceToNextThousand;

    m_zDistance += distanceToNextThousand <= (s_chunkLength / 2.f) ? s_chunkLength : 2 * s_chunkLength;
}

void ChunkGenerator::createBoostDistribution()
{
    m_numUsedBoostDistributions = 0;
    int numRemainingBoosts = glm::smoothstep(s_startIncreasingSeverity, s_stopIncreasingSeverity, m_zDistance) * 5.f + 5.f; // [5, 10]
    std::uniform_int_distribution<> dist(0, 2);

    for (int i = 0; i < s_chunksPerBoostDistribution - 1; ++i)
    {
        if (numRemainingBoosts > 0)
        {
            int numBoosts = std::min(dist(m_generator), numRemainingBoosts);
            m_boostDistribution[i] = numBoosts;
            numRemainingBoosts -= numBoosts;
        }
        else 
        {
            m_boostDistribution[i] = 0;
        }
    }

    m_boostDistribution[s_chunksPerBoostDistribution - 1] = std::min(numRemainingBoosts, 2);
}
