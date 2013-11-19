#include "Cuboid.h"

Cuboid::Cuboid(const glm::vec3 & size)
:   m_size(size)
{
}

Cuboid::Cuboid(const glm::vec3 & size, const glm::mat4 & matrix)
:   m_size(size)
,   m_matrix(matrix)
{
}

Cuboid::~Cuboid()
{
}

const glm::vec3 & Cuboid::size() const
{
    return m_size;
}

const glm::mat4 & Cuboid::matrix() const
{
    return m_matrix;
}

void Cuboid::setMatrix(const glm::mat4 & matrix)
{
    m_matrix = matrix;
}
