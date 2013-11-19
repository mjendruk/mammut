
#include "Quad.h"

Quad::Quad(const glm::vec3 & size)
:   m_size(size)
{
}

Quad::Quad(const glm::vec3 & size, const glm::mat4 & matrix)
:   m_size(size)
,   m_matrix(matrix)
{
}

Quad::~Quad()
{
}

const glm::vec3 & Quad::size() const
{
	return m_size;
}

const glm::mat4 & Quad::matrix() const
{
    return m_matrix;
}

void Quad::setMatrix(const glm::mat4 & matrix)
{
    m_matrix = matrix;
}
