#include "ParticleGenerator.h"

#include <iostream>

#include <glm/gtx/random.hpp>


ParticleGenerator::ParticleGenerator()
{

}

void ParticleGenerator::update(const glm::vec3 & eye)
{
    static const int maxNumParticles = 200;
    static const float zOffset = 2.0f;
    
    m_particles.remove_if([&eye] (const glm::vec3 & particle) {
        return particle.z > eye.z + zOffset;
    });
    
    int i = 0;
    int j = 0;
    
    while (m_particles.size() < maxNumParticles)
    {
        i++;
        glm::vec3 newParticle = glm::vec3(glm::gaussRand(eye, glm::vec3(2.0f)));
        
        if (newParticle.z < eye.z + zOffset && 
            glm::distance(newParticle.xy(), eye.xy()) > 2.0f)
        {
            j++;
            m_particles.push_back(newParticle);
        }
    }
    
    std::cout << i << " " << j << " " << std::endl;
}

void ParticleGenerator::reset()
{
    m_particles.clear();
}

std::vector<glm::vec3> ParticleGenerator::particles()
{
    std::vector<glm::vec3> vector;
    vector.reserve(m_particles.size());
    vector.insert(vector.end(), m_particles.begin(), m_particles.end());
    return vector;
}
