#pragma once

#include <list>
#include <vector>

#include <glm/glm.hpp>


class ParticleGenerator
{
public:
    ParticleGenerator();

    void update(const glm::vec3 & eye, const glm::vec3 & center);

    std::vector<glm::vec3> particles();

private:
    std::list<glm::vec3> m_particles;
};
