#include "ChunkGenerator.h"

#include <glm/glm.hpp>

#include "Cuboid.h"

ChunkGenerator::ChunkGenerator(int seed)
:   m_generator(seed)
,   m_nextTranslation(0.0f, 0.0f, 0.0f)
{

}

ChunkGenerator::~ChunkGenerator()
{

}

QSharedPointer<CuboidChunk> ChunkGenerator::nextChunk()
{
    QSharedPointer<CuboidChunk> chunk(new CuboidChunk);
    
    std::normal_distribution<> sizeXYDistribution(10, 4);
    std::uniform_real_distribution<> sizeZDistribution(25.0f, 50.0f);
    std::uniform_real_distribution<> positionXYDistribution(-35.0f, 35.0f);
    std::uniform_real_distribution<> positionZDistribution(0.0f, 30.0f);
    
    for (int i = 0; i < 20; ++i)
    {
        const glm::vec3 size(sizeXYDistribution(m_generator),
                             sizeXYDistribution(m_generator),
                             sizeZDistribution(m_generator));
        
        const glm::vec3 position(positionXYDistribution(m_generator),
                                 positionXYDistribution(m_generator),
                                 -positionZDistribution(m_generator));
        
        chunk->add(new Cuboid(size, position + m_nextTranslation));
    }
    
    m_nextTranslation += glm::vec3(0.0f, 0.0f, -70.0f);
    return chunk;
}

void ChunkGenerator::addZShift(float zShift)
{
    m_nextTranslation = m_nextTranslation + glm::vec3(0.0f, 0.0f, zShift);
}
