#include "ChunkGenerator.h"

#include <glm/glm.hpp>
#include "Cuboid.h"

ChunkGenerator::ChunkGenerator(int seed)
:   m_seed(seed)
,   m_nextTranslation(0.0f, 0.0f, 0.0f)
{

}

ChunkGenerator::~ChunkGenerator()
{

}

QSharedPointer<CuboidChunk> ChunkGenerator::nextChunk()
{
    QSharedPointer<CuboidChunk> chunk(new CuboidChunk);
    
    //1 
    chunk->add(new Cuboid(glm::vec3(10.0f, 8.0f, 45.0f), glm::vec3(-31.0f, 21.0f, 3.0f) + m_nextTranslation));
    //2
    chunk->add(new Cuboid(glm::vec3(9.0f, 10.0f, 48.0f), glm::vec3(-1.0f, 32.0f, -15.0f) + m_nextTranslation));    
    //3
    chunk->add(new Cuboid(glm::vec3(11.0f, 10.0f, 64.0f), glm::vec3(13.0f, 28.0f, -6.0f) + m_nextTranslation));    
    //4
    chunk->add(new Cuboid(glm::vec3(9.0f, 9.5f, 22.0f), glm::vec3(-20.0f, 11.0f, -50.0f) + m_nextTranslation));
    //5 (0,0,0)
    chunk->add(new Cuboid(glm::vec3(9.0f, 15.0f, 66.0f), glm::vec3(0.0f, 0.0f, 0.0f) + m_nextTranslation));
    //6
    chunk->add(new Cuboid(glm::vec3(10.0f, 8.0f, 36.0f), glm::vec3(14.0f, -10.0f, -18.0f) + m_nextTranslation));
    //7
    chunk->add(new Cuboid(glm::vec3(9.0f, 10.0f, 57.0f), glm::vec3(-38.0f, -12.0f, 8.0f) + m_nextTranslation));
    //8
    chunk->add(new Cuboid(glm::vec3(9.0f, 10.5f, 30.0f), glm::vec3(-4.0f, -22.0f, -37.0f) + m_nextTranslation));
    //9
    chunk->add(new Cuboid(glm::vec3(12.2f, 15.0f, 50.0f), glm::vec3(-26.0f, -7.0f, -7.0f) + m_nextTranslation));
    //10
    chunk->add(new Cuboid(glm::vec3(10.0f, 7.0f, 27.0f), glm::vec3(25.0f, 5.0f, -30.0f) + m_nextTranslation));
    
    //11
    chunk->add(new Cuboid(glm::vec3(13.0f, 11.0f, 19.0f), glm::vec3(-29.0f, 40.0f, -29.0f) + m_nextTranslation));
    //12
    chunk->add(new Cuboid(glm::vec3(9.0f, 9.0f, 27.0f), glm::vec3(-12.0f, -34.0f, -30.0f) + m_nextTranslation));
    //13
    chunk->add(new Cuboid(glm::vec3(10.0f, 12.0f, 27.0f), glm::vec3(32.0f, -24.0f, -30.0f) + m_nextTranslation));
    //14
    chunk->add(new Cuboid(glm::vec3(7.0f, 8.0f, 27.0f), glm::vec3(22.0f, -2.0f, -30.0f) + m_nextTranslation));    
    //15
    chunk->add(new Cuboid(glm::vec3(11.0f, 7.0f, 33.0f), glm::vec3(-16.0f, 5.0f, -7.0f) + m_nextTranslation));

    
    m_nextTranslation += glm::vec3(0.0f, -1.0f, -70.0f);
    return chunk;
}
