#include "CaveDrawable.h"

#include <cassert>

#define _USE_MATH_DEFINES
#include <cmath>

#include <glow/VertexArrayObject.h>
#include <glow/Buffer.h>
#include <glow/VertexAttributeBinding.h>

#include <QDebug>

const int CaveDrawable::s_verticesPerRing = 4;
const float CaveDrawable::s_radius = 100.f;
const glm::vec3 CaveDrawable::s_maxShift = glm::vec3(0.f); //glm::vec3(1.f, 1.f, 1.f);
const int CaveDrawable::s_numRings = 4;

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
    m_numIndices = 0;
    glow::Array<glm::vec3> ar;
    glow::Array<glm::vec3> arOffset;

    for (int i = 0; i < s_verticesPerRing; i++) {
        float angle = (3.14159265359f * 2 / s_verticesPerRing) * i;
        ar << glm::vec3(cos(angle), sin(angle), 0.0) * s_radius;

        float angleOffset = (3.14159265359f * 2 / s_verticesPerRing) * (i + 0.5f);
        arOffset << glm::vec3(cos(angleOffset), sin(angleOffset), 0.0) * s_radius;
    }

    for (int i = 0; i < s_numRings / 2; i++) {
        for (glm::vec3 v : ar){
            m_vertices << v + glm::vec3(0.0f, 0.0f, i * 10);
            glm::vec3 a = v + glm::vec3(0.0f, 0.0f, i * 10);
            qDebug() << a.x << " " << a.y << " " << a.z;
            ++m_numIndices;
        }

        for (glm::vec3 v : arOffset){
            m_vertices << v + glm::vec3(0.0f, 0.0f, (i + 0.5f) * 10);
            glm::vec3 a = v + glm::vec3(0.0f, 0.0f, (i + 0.5f) * 10);
            qDebug() << a.x << " " << a.y << " " << a.z;
            ++m_numIndices;
        }
         
    }

    int countVertexOnRing = 0;
    int ring = 0;
    int offset = 0;

    for (int vertex = 0; vertex < s_numRings * (s_verticesPerRing - 1); ++vertex)
    {
        if (ring % 2 == 0)
            offset = 0;
        else
            offset = 1;

        ++countVertexOnRing;
        m_indices << vertex;
        m_indices << vertex + s_verticesPerRing + offset;

        qDebug() << vertex << " <-> " << vertex + s_verticesPerRing + offset;

        if (countVertexOnRing == 4)
        {
            m_indices << vertex - (s_verticesPerRing - 1);
            m_indices << vertex + 1 + offset;

            qDebug() << vertex - (s_verticesPerRing - 1) << " <-> " << vertex + 1 + offset;

            m_indices << 50000;
            qDebug() << "Restart";

            countVertexOnRing = 0;

            ring = (ring + 1) % 2;
        }
    }
}

void CaveDrawable::draw()
{
    if (m_vao == nullptr)
        return;
    m_vao->bind();
    glPrimitiveRestartIndex(50000);
    int size = s_verticesPerRing * s_numRings * 3;
    m_vao->drawElements(GL_TRIANGLE_STRIP, m_numIndices, GL_UNSIGNED_INT, nullptr);
    m_vao->unbind();
}