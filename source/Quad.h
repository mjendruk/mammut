
#pragma once

#include <glm/glm.hpp>

class Quad
{
public:
    Quad(const glm::vec3 & size);
    
    Quad(const glm::vec3 & size,
         const glm::mat4 & matrix);
         
    ~Quad();

	const glm::vec3 & size() const;
    const glm::mat4 & matrix() const;
    void setMatrix(const glm::mat4 & matrix);

protected:
    glm::mat4 m_matrix;
    glm::vec3 m_size;
};
