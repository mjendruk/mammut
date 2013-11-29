
#include "ChunkGenerator.h"

#include <glm/glm.hpp>
#include "Cuboid.h"

ChunkGenerator::ChunkGenerator(int seed, btDiscreteDynamicsWorld & dynamicsWorld)
:   m_seed(seed)
,   m_nextTranslation(0.0f, 0.0f, 0.0f)
,   m_dynamicsWorld(dynamicsWorld)
{

}

ChunkGenerator::~ChunkGenerator()
{

}

QSharedPointer<CuboidChunk> ChunkGenerator::nextChunk()
{
    QSharedPointer<CuboidChunk> chunk(new CuboidChunk);
    
    chunk->add(new Cuboid(glm::vec3(4.0f, 4.0f, 20.0f), glm::vec3(-8.0f, 4.0f, 0.0f) + m_nextTranslation, m_dynamicsWorld));
    chunk->add(new Cuboid(glm::vec3(4.0f, 4.0f, 20.0f), glm::vec3(0.0f, 0.0f, 0.0f) + m_nextTranslation, m_dynamicsWorld));
    
    m_nextTranslation += glm::vec3(0.0f, -2.0f, -23.f);
    return chunk;
}
