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
    const glm::mat4 & modelMatrix() const;
    void setModelMatrix(const glm::mat4 & modelMatrix);

protected:
    glm::mat4 m_modelMatrix;
    glm::vec3 m_size;
};
