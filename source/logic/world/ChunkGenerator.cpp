#include "ChunkGenerator.h"

#include <algorithm>
#include <chrono>
#include <cassert>

#include <QDebug>

#include <glm/glm.hpp>

#include "Cuboid.h"

ChunkGenerator::ChunkGenerator(int seed)
:   m_generator(std::chrono::system_clock::now().time_since_epoch().count())
,   m_nextTranslation(0.0f, 0.0f, 0.0f)
,   m_zDistance(0.0)
{

}

ChunkGenerator::~ChunkGenerator()
{

}

QSharedPointer<CuboidChunk> ChunkGenerator::nextChunk()
{
    QSharedPointer<CuboidChunk> chunk(new CuboidChunk);
    
    if (m_nextTranslation.z == 0.f) {
        chunk->add(new Cuboid(glm::vec3(20.f, 10.f, 80.f), glm::vec3(0.f, -2.f, -20.f)));
        m_nextTranslation += glm::vec3(0.0f, 0.0f, -70.0f);
        return chunk;
    }

    std::normal_distribution<> sizeXYDistribution(10, 4); // too big is alwas a problem?
    std::uniform_real_distribution<> sizeZDistribution(40.0f, 60.0f);
    std::uniform_real_distribution<> positionXYDistribution(-35.0f, 35.0f);
    std::uniform_real_distribution<> positionZDistribution(0.0f, 30.0f);
    
    int numCuboids = glm::smoothstep(200.0, 5000.0, m_zDistance) * 40.0 + 10.0 ;

    qDebug() << "----------------------";
    qDebug() << "num Cuboids" << numCuboids;

    for (int i = 0; i < /*numCuboids*/50; ++i)
    {
        float xSize = std::max(float(sizeXYDistribution(m_generator)), 2.0f);
        float ySize = std::max(float(sizeXYDistribution(m_generator)), 2.0f);
        const glm::vec3 size(xSize,
                             ySize,
                             sizeZDistribution(m_generator));
        
        const glm::vec3 position(positionXYDistribution(m_generator),
                                 positionXYDistribution(m_generator),
                                 -positionZDistribution(m_generator));
        
        chunk->add(new Cuboid(size, position + m_nextTranslation));
    }

    int maxOverlaps = 15;//int(glm::smoothstep(200.0, 1000.0, m_zDistance) * 15.0); //[0 , 15]
    int numOverlaps = 0;
    int numDeletions = 0;

    QList<Cuboid *> cuboidList = chunk->cuboids();
    QList<int> removeIndexList;

    for (int i = 0; i < cuboidList.size()-1; i++)
    for (int j = i + 1; j < cuboidList.size(); j++)
    {
        glm::vec2 c1URB = cuboidList.at(i)->boundingBox().urb().xy;
        glm::vec2 c1LLF = cuboidList.at(i)->boundingBox().llf().xy;

        glm::vec2 c2URB = cuboidList.at(j)->boundingBox().urb().xy;
        glm::vec2 c2LLF = cuboidList.at(j)->boundingBox().llf().xy;

        bool overlapped = !(c1LLF.x > c2URB.x || c2LLF.x > c1URB.x || c2LLF.y > c1URB.y || c1LLF.y > c2URB.y);

        if(overlapped) {
            ++numOverlaps;
            float minOverlap = 1.f;

            float minDeltaX = std::min(glm::abs(c1URB.x - c2URB.x), glm::abs(c1LLF.x - c2LLF.x));
            float minDeltaY = std::min(glm::abs(c1URB.y - c2URB.y), glm::abs(c1LLF.y - c2LLF.y));
            if (numOverlaps > maxOverlaps)
                qDebug() << "too much overlaps";
            if (minDeltaX < minOverlap || minDeltaY < minOverlap || numOverlaps > maxOverlaps)
                removeIndexList << i; ++numDeletions; break;

        }
    }

    for (int i = removeIndexList.size() - 1; i >= 0; --i)
        chunk->remove(removeIndexList.at(i));

    qDebug() << "num overlaps" << numOverlaps;
    qDebug() << "num deletions" << numDeletions;
        
    m_nextTranslation += glm::vec3(0.0f, 0.0f, -70.0f);
    m_zDistance += 70.0;
    return chunk;
}
