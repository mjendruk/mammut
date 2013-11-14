
#include "Quad.h"

#include <glow/VertexArrayObject.h>
#include <glow/Buffer.h>
#include <glow/VertexAttributeBinding.h>

Quad::Quad()
:   m_vao(new glow::VertexArrayObject())
,   m_vertices(new glow::Buffer(GL_ARRAY_BUFFER))
,   m_indices(new glow::Buffer(GL_ELEMENT_ARRAY_BUFFER))
{
    initialize();
}

Quad::Quad(const glm::mat4 & matrix)
:   m_vao(new glow::VertexArrayObject())
,   m_vertices(new glow::Buffer(GL_ARRAY_BUFFER))
,   m_indices(new glow::Buffer(GL_ELEMENT_ARRAY_BUFFER))
,   m_matrix(matrix)
{
    initialize();
}

void Quad::initialize()
{
    auto v(vertices());
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

glow::Vec3Array Quad::vertices()
{
    return glow::Vec3Array
    {
        glm::vec3( .5f,  .5f,  .5f),
        glm::vec3(-.5f,  .5f,  .5f),
        glm::vec3(-.5f, -.5f,  .5f),
        glm::vec3( .5f, -.5f,  .5f),
        glm::vec3( .5f,  .5f, -.5f),
        glm::vec3(-.5f,  .5f, -.5f),
        glm::vec3(-.5f, -.5f, -.5f),
        glm::vec3( .5f, -.5f, -.5f)
    };
}

glow::Array<glm::lowp_uvec3> Quad::indices()
{
    return glow::Array<glm::lowp_uvec3>
    {
        glm::lowp_uvec3(0, 1, 2),
        glm::lowp_uvec3(0, 3, 2),
        glm::lowp_uvec3(0, 1, 5),
        glm::lowp_uvec3(0, 4, 5),
        glm::lowp_uvec3(0, 4, 7),
        glm::lowp_uvec3(0, 3, 7),
        glm::lowp_uvec3(6, 2, 1),
        glm::lowp_uvec3(6, 5, 1),
        glm::lowp_uvec3(6, 5, 4),
        glm::lowp_uvec3(6, 7, 4),
        glm::lowp_uvec3(6, 2, 3),
        glm::lowp_uvec3(6, 7, 3)
    };
}
