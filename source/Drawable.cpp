
#include "Drawable.h"

#include <cassert>

#include <glow/VertexArrayObject.h>
#include <glow/Buffer.h>
#include <glow/VertexAttributeBinding.h>

Drawable::Drawable()
:   m_vao(nullptr)
,   m_vertices(nullptr)
,   m_indices(nullptr)
{

}

Drawable::~Drawable()
{
}

void Drawable::initialize()
{
    m_vao = new glow::VertexArrayObject();
    m_vertices = new glow::Buffer(GL_ARRAY_BUFFER);
    m_indices = new glow::Buffer(GL_ELEMENT_ARRAY_BUFFER);

    auto v(vertices());
    auto i(indices());

    m_vertices->setData(v, GL_STATIC_DRAW);
    m_indices->setData(i, GL_STATIC_DRAW);

    m_vao->bind();

    auto vertexBinding = m_vao->binding(0);
    vertexBinding->setAttribute(0);
    vertexBinding->setBuffer(m_vertices, 0, sizeof(glm::vec3));
    vertexBinding->setFormat(3, GL_FLOAT);
    m_vao->enable(0);

    m_indices->bind();

    m_vao->unbind();
}

void Drawable::draw()
{
    assert(m_vao.get() != nullptr);

    glEnable(GL_DEPTH_TEST);

    m_vao->bind();
    m_vao->drawElements(GL_TRIANGLES, indices().size() * 3, GL_UNSIGNED_SHORT, nullptr);
    m_vao->unbind();
}

glow::Vec3Array Drawable::vertices()
{
    return glow::Vec3Array
    {
        glm::vec3(0.f, 0.f, 0.f),
        glm::vec3(1.f, 0.f, 0.f),
        glm::vec3(1.f, 1.f, 0.f),
        glm::vec3(0.f, 1.f, 0.f),
        glm::vec3(0.f, 0.f, -1.f),
        glm::vec3(1.f, 0.f, -1.f),
        glm::vec3(1.f, 1.f, -1.f),
        glm::vec3(0.f, 1.f, -1.f),
    };
}

glow::Array<glm::lowp_uvec3> Drawable::indices()
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
