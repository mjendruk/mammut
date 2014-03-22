#include "ParticleGenerator.h"

#include <iostream>

#include <glm/gtx/random.hpp>
#include <glm/gtx/string_cast.hpp>


ParticleGenerator::ParticleGenerator()
{

}

void ParticleGenerator::update(const glm::vec3 & eye, const glm::vec3 & center)
{
    static const int maxNumParticles = 200;

    const glm::vec3 lookAt = glm::normalize(center - eye);

    std::cout << "pre: " << m_particles.size() << std::endl;
    
    m_particles.remove_if([&lookAt, &eye] (const glm::vec3 & particle) {
        return glm::dot(lookAt, particle - eye) < 0;
    });
    
    std::cout << "after: " << m_particles.size() << std::endl;

    while (m_particles.size() < maxNumParticles)
    {
        glm::vec3 newParticle = glm::vec3(glm::gaussRand(eye, glm::vec3(2.0f)));

        if (glm::distance(eye, newParticle) >= 2.0f)
            m_particles.push_back(newParticle);
    }
}

std::vector<glm::vec3> ParticleGenerator::particles()
{
    std::vector<glm::vec3> vector;
    vector.reserve(m_particles.size());
    vector.insert(vector.end(), m_particles.begin(), m_particles.end());
    return vector;
}
