#include "CaveDrawable.h"

#include <cassert>

#define _USE_MATH_DEFINES
#include <cmath>

#include <glow/VertexArrayObject.h>
#include <glow/Buffer.h>
#include <glow/VertexAttributeBinding.h>

const int CaveDrawable::s_verticesPerRing = 1;
const float CaveDrawable::s_radius = 100.f;
const glm::vec3 CaveDrawable::s_maxShift = glm::vec3(1.f, 1.f, 1.f);
const int CaveDrawable::s_numRings = 30;

CaveDrawable::CaveDrawable()
:   m_vertexBuffer(nullptr)
,   m_normalBuffer(nullptr)
,   m_indexBuffer(nullptr)
,   m_vao(nullptr)
{

}

CaveDrawable::~CaveDrawable()
{
}

void CaveDrawable::initialize()
{
    const GLuint vertexAttribLocation = 0;
    const GLuint normalAttribLocation = 1;

    m_vao = new glow::VertexArrayObject();

    m_vertexBuffer = new glow::Buffer(GL_ARRAY_BUFFER);
    m_normalBuffer = new glow::Buffer(GL_ARRAY_BUFFER);
    m_indexBuffer = new glow::Buffer(GL_ELEMENT_ARRAY_BUFFER);

    initializeData();
    m_vertexBuffer->setData(m_vertices, GL_STATIC_DRAW);
    m_vertexBuffer->setData(m_normals, GL_STATIC_DRAW);
    m_indexBuffer->setData(m_indices, GL_STATIC_DRAW);

    m_vao->bind();

    auto vertexBinding = m_vao->binding(0);
    vertexBinding->setAttribute(vertexAttribLocation);
    vertexBinding->setBuffer(m_vertexBuffer, 0, sizeof(glm::vec3));
    vertexBinding->setFormat(3, GL_FLOAT, GL_FALSE);
    m_vao->enable(0);

    auto normalBinding = m_vao->binding(1);
    normalBinding->setAttribute(normalAttribLocation);
    normalBinding->setBuffer(m_normalBuffer, 0, sizeof(glm::vec3));
    normalBinding->setFormat(3, GL_FLOAT, GL_TRUE);
    m_vao->enable(1);

    m_indexBuffer->bind();

    m_vao->unbind();


}

void CaveDrawable::initializeData()
{
    glow::Array<glm::vec3> ar;
    glow::Array<glm::vec3> arOffset;

    for (int i = 0; i < s_verticesPerRing; i++) {
        float angle = (3.14159265359f * 2 / s_verticesPerRing) * i;
        ar << glm::vec3(cos(angle), sin(angle), 0.0) * s_radius;

        float angleOffset = (3.14159265359f * 2 / s_verticesPerRing) * (i + 0.5f);
        arOffset << glm::vec3(cos(angleOffset), sin(angleOffset), 0.0) * s_radius;
    }

    for (int i = 0; i < s_numRings / 2; i++) {
        for (glm::vec3 v : ar)
            m_vertices << v + glm::vec3(0.0f, 0.0f, i * 10);

        for (glm::vec3 v : arOffset)
            m_vertices << v + glm::vec3(0.0f, 0.0f, (i + 0.5f) * 10);
    }

    for (int i = 0; i < s_numRings; i++) {
        for (int j = 0; j < s_verticesPerRing; j++) {

        }
    }
}

void CaveDrawable::draw()
{
    m_vao->bind();
    //m_vao->drawElements(GL_TRIANGLE_STRIP, m_size, GL_UNSIGNED_SHORT, nullptr);
    m_vao->unbind();
}