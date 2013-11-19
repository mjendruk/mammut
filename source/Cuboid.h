
#pragma once

#include <glm/glm.hpp>

class Cuboid
{
public:
    Cuboid(const glm::vec3 & size);
    
    Cuboid(const glm::vec3 & size,
         const glm::mat4 & matrix);
         
    ~Cuboid();

    const glm::vec3 & size() const;
    const glm::mat4 & matrix() const;
    void setMatrix(const glm::mat4 & matrix);

protected:
    glm::mat4 m_matrix;
    glm::vec3 m_size;
};
