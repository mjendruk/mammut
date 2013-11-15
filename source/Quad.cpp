
#include "Quad.h"

#include <glow/VertexArrayObject.h>
#include <glow/Buffer.h>
#include <glow/VertexAttributeBinding.h>
#include <glowutils/AxisAlignedBoundingBox.h>

Quad::Quad(const glm::vec3 & size)
:   m_vao(new glow::VertexArrayObject())
,   m_vertices(new glow::Buffer(GL_ARRAY_BUFFER))
,   m_indices(new glow::Buffer(GL_ELEMENT_ARRAY_BUFFER))
{
    initialize(size);
}

Quad::Quad(const glm::vec3 & size,
    const glm::mat4 & matrix)
:   m_vao(new glow::VertexArrayObject())
,   m_vertices(new glow::Buffer(GL_ARRAY_BUFFER))
,   m_indices(new glow::Buffer(GL_ELEMENT_ARRAY_BUFFER))
,   m_matrix(matrix)
{
    initialize(size);
}

void Quad::initialize(const glm::vec3 & size)
{
    auto v(vertices(size));
    auto i(indices());
    
    m_vertices->setData(v, GL_STATIC_DRAW);
    m_indices->setData(i, GL_STATIC_DRAW);
    
    m_size = i.size() * 3;
    
    m_vao->bind();
    
    auto vertexBinding = m_vao->binding(0);
    vertexBinding->setAttribute(0);
    vertexBinding->setBuffer(m_vertices, 0, sizeof(glm::vec3));
    vertexBinding->setFormat(3, GL_FLOAT);
    m_vao->enable(0);
    
    m_indices->bind();
    
    m_vao->unbind();
}

Quad::~Quad()
{
}

void Quad::draw()
{
    glEnable(GL_DEPTH_TEST);
    
    m_vao->bind();
    m_vao->drawElements(GL_TRIANGLES, m_size, GL_UNSIGNED_SHORT, nullptr);
    m_vao->unbind();
}

const glm::mat4 & Quad::matrix()
{
    return m_matrix;
}

void Quad::setMatrix(const glm::mat4 & matrix)
{
    m_matrix = matrix;
}

glow::Vec3Array Quad::vertices(const glm::vec3 & size)
{
    return glow::Vec3Array
    {
        glm::vec3(   0.f,    0.f,     0.f),
        glm::vec3(size.x,    0.f,     0.f),
        glm::vec3(size.x, size.y,     0.f),
        glm::vec3(   0.f, size.y,     0.f),
        glm::vec3(   0.f,    0.f, -size.z),
        glm::vec3(size.x,    0.f, -size.z),
        glm::vec3(size.x, size.y, -size.z),
        glm::vec3(   0.f, size.y, -size.z),
    };
}

glow::Array<glm::lowp_uvec3> Quad::indices()
{
    return glow::Array<glm::lowp_uvec3>
    {
        glm::lowp_uvec3(0, 1, 2),
        glm::lowp_uvec3(0, 2, 3),
        glm::lowp_uvec3(0, 1, 5),
        glm::lowp_uvec3(0, 5, 4),
        glm::lowp_uvec3(0, 4, 7),
        glm::lowp_uvec3(0, 7, 3),
        glm::lowp_uvec3(6, 5, 1),
        glm::lowp_uvec3(6, 1, 2),
        glm::lowp_uvec3(6, 2, 3),
        glm::lowp_uvec3(6, 3, 7),
        glm::lowp_uvec3(6, 5, 4),
        glm::lowp_uvec3(6, 4, 7)
    };
}
