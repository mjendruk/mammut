#include "Cuboid.h"

Cuboid::Cuboid(const glm::vec3 & size)
:   m_size(size)
{
}

Cuboid::Cuboid(const glm::vec3 & size, const glm::mat4 & modelMatrix)
:   m_size(size)
,   m_modelMatrix(modelMatrix)
{
}

Cuboid::~Cuboid()
{
}

const glm::vec3 & Cuboid::size() const
{
    return m_size;
}

const glm::mat4 & Cuboid::modelMatrix() const
{
    return m_modelMatrix;
}

void Cuboid::setModelMatrix(const glm::mat4 & matrix)
{
    m_modelMatrix = matrix;
}
