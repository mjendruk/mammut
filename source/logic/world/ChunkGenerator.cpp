#include "ChunkGenerator.h"

#include <algorithm>
#include <chrono>
#include <cassert>

#include <QDebug>

#include <glm/glm.hpp>

#include "Cuboid.h"

ChunkGenerator::ChunkGenerator(int seed)
:   m_generator(std::chrono::system_clock::now().time_since_epoch().count())
,   m_zDistance(0.0)
{

}

ChunkGenerator::~ChunkGenerator()
{

}

QSharedPointer<CuboidChunk> ChunkGenerator::nextChunk()
{
    QSharedPointer<CuboidChunk> chunk(new CuboidChunk);
    //distance from last cuboid to wall: 200
    
    if (m_zDistance == 0.f) {
        chunk->add(new Cuboid(glm::vec3(20.f, 10.f, 80.f), glm::vec3(0.f, -2.f, -20.f - m_zDistance)));
        m_zDistance += 70.f;
        return chunk;
    }
    
    const int wallStep = 1000;

    float distanceToNextThousand = wallStep - int(m_zDistance) % wallStep;
    if (distanceToNextThousand <= 70.f) {
        createWall(*chunk.data(), distanceToNextThousand, (m_zDistance > 2 * wallStep ? false : true));
        return chunk;
    }
        
    createOrdinaryLevel(*chunk.data());
    return chunk;
    
    
}

void ChunkGenerator::createOrdinaryLevel(CuboidChunk & chunk)
{
    int numCuboids = glm::smoothstep(200.0, 3000.0, m_zDistance) * 46.0 + 4.0; // [4, 50]
    float xyDistribution = glm::smoothstep(200.0, 3000.0, m_zDistance) * 15 + 20; // [20, 35]
    int maxOverlaps = int(glm::smoothstep(200.0, 3000.0, m_zDistance) * 18.0); //[0 , 18]
    float minSizeZDistribution = glm::smoothstep(200.0, 3000.0, m_zDistance) * 20; // [0, 20]

    qDebug() << "----------------------";
    qDebug() << "num Cuboids: " << numCuboids << "   num of max. allowed overlaps: " << maxOverlaps;

    std::normal_distribution<> sizeXYDistribution(10, 4); // too big is no fun
    std::uniform_real_distribution<> sizeZDistribution(50.0f - minSizeZDistribution, 60.0f); //30 60 is difficult with 50 cuboids + 18 overlap + -35->35||
    std::uniform_real_distribution<> positionXYDistribution(-xyDistribution, xyDistribution);//-35->35+ 50 60 is easy to middle + 5 cubs + 0 overlap 
    std::uniform_real_distribution<> positionZDistribution(0.0f, 30.0f);

    for (int i = 0; i < numCuboids; ++i)
    {
        float xSize = std::max(float(sizeXYDistribution(m_generator)), 2.0f);
        float ySize = std::max(float(sizeXYDistribution(m_generator)), 2.0f);
        const glm::vec3 size(xSize,
            ySize,
            sizeZDistribution(m_generator));

        const glm::vec3 position(positionXYDistribution(m_generator),
            positionXYDistribution(m_generator),
            -positionZDistribution(m_generator));

        chunk.add(new Cuboid(size, position - glm::vec3(0.f, 0.f, m_zDistance)));
    }

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
            float minOverlap = 1.f;

            float minDeltaX = std::min(glm::abs(c1URB.x - c2URB.x), glm::abs(c1LLF.x - c2LLF.x));
            float minDeltaY = std::min(glm::abs(c1URB.y - c2URB.y), glm::abs(c1LLF.y - c2LLF.y));
            if ((minDeltaX < minOverlap || minDeltaY < minOverlap) && !(numOverlaps > maxOverlaps))
                qDebug() << "   " << "too small overlap -> delete cuboid";
            if ((numOverlaps > maxOverlaps) && !(minDeltaX < minOverlap || minDeltaY < minOverlap))
                qDebug() << "   " << "too many overlaps -> delete cuboid";
            if ((numOverlaps > maxOverlaps) && (minDeltaX < minOverlap || minDeltaY < minOverlap))
                qDebug() << "   " << "too many overlaps & too small overlap -> delete cuboid";

            if ((minDeltaX < minOverlap) || (minDeltaY < minOverlap) || (numOverlaps > maxOverlaps)) {
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

    m_zDistance += 70.0;
}

void ChunkGenerator::createWall(CuboidChunk & chunk, float distanceToNextThousand, bool createStripe)
{
    const float wallSize = 500.f;
    const float thickness = 5.f;

    const float zPosition = -(m_zDistance + distanceToNextThousand + thickness / 2.f);

    std::uniform_real_distribution<> offsetDistribution(-30.0f, 30.0f);
    std::normal_distribution<> sizeDistribution(15.0f, 4.0f);

    float sizeX = std::max(3.f, float(sizeDistribution(m_generator)));
    float sizeY = std::max(3.f, float(sizeDistribution(m_generator)));

    float offsetX = float(offsetDistribution(m_generator));
    float offsetY = float(offsetDistribution(m_generator));

    chunk.add(new Cuboid(glm::vec3(wallSize, wallSize, 5.0f), glm::vec3(-(wallSize + sizeX) / 2.f + offsetX, offsetY, zPosition)));
    chunk.add(new Cuboid(glm::vec3(wallSize, wallSize, 5.0f), glm::vec3(+(wallSize + sizeX) / 2.f + offsetX, offsetY, zPosition)));

    if (!createStripe)
    {
        chunk.add(new Cuboid(glm::vec3(sizeX, wallSize, 5.0f), glm::vec3(offsetX, -(wallSize + sizeY) / 2.f + offsetY, zPosition)));
        chunk.add(new Cuboid(glm::vec3(sizeX, wallSize, 5.0f), glm::vec3(offsetX, +(wallSize + sizeY) / 2.f + offsetY, zPosition)));
    }

    qDebug() << "----------------------";
    qDebug() << "wall " << (createStripe ? "(stripe)" : "(no stripe)");
    qDebug() << "size: " << sizeX << " x " << sizeY;
    qDebug() << "offset: " << offsetX << " x " << offsetY;
    qDebug() << "distance: " << m_zDistance + distanceToNextThousand << "     distance to last chunk: " << distanceToNextThousand;

    m_zDistance += distanceToNextThousand <= 35.f ? 70.f : 140.f;
}