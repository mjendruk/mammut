#include "CaveDrawable.h"

#include <cassert>

#define _USE_MATH_DEFINES
#include <cmath>

#include <glow/VertexArrayObject.h>
#include <glow/Buffer.h>
#include <glow/VertexAttributeBinding.h>

#include <QDebug>

const int CaveDrawable::s_verticesPerRing = 8;
const float CaveDrawable::s_radius = 100.f;
const glm::vec3 CaveDrawable::s_maxShift = glm::vec3(0.f); //glm::vec3(1.f, 1.f, 1.f);
const int CaveDrawable::s_numRings = 100;

CaveDrawable::CaveDrawable()
:   m_vertexBuffer(nullptr)
,   m_normalBuffer(nullptr)
,   m_indexBuffer(nullptr)
,   m_vao(nullptr)
,   m_ringZStride(-50.f)
,   m_activeRingPosition(0.f)
{
    for (int i = 0; i < s_verticesPerRing; i++) {
        float angle = (3.14159265359f * 2 / s_verticesPerRing) * i;
        dummyArray << glm::vec3(cos(angle), sin(angle), 0.0) * s_radius;

        float angleOffset = (3.14159265359f * 2 / s_verticesPerRing) * (i + 0.5f);
        dummyArrayOffset << glm::vec3(cos(angleOffset), sin(angleOffset), 0.0) * s_radius;
    }
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
    m_normalBuffer->setData(m_normals, GL_STATIC_DRAW);
    m_indexBuffer->setData(m_indices, GL_STATIC_DRAW);

    m_vao->bind();
    glEnable(GL_PRIMITIVE_RESTART);

    glPrimitiveRestartIndex(56789);

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
    writeVertexList(s_numRings);

    int countVertexOnRing = 0;
    int ring = 0;
    int offset = 0;

    for (int vertex = 0; vertex < (s_numRings-1) * (s_verticesPerRing); ++vertex)
    {
        if (ring % 2 == 0)
            offset = 0;
        else
            offset = 1;

        ++countVertexOnRing;
        m_indices << vertex;

        if (countVertexOnRing == s_verticesPerRing && (ring % 2) == 1)
        {
            m_indices << vertex + offset;
            qDebug() << vertex << " <-> " << vertex + offset;
        }
        else
        {
            m_indices << vertex + s_verticesPerRing + offset;
            qDebug() << vertex << " <-> " << vertex + s_verticesPerRing + offset;
        }

        if (countVertexOnRing == s_verticesPerRing)
        {
            //close ring
            m_indices << vertex - (s_verticesPerRing - 1);
            m_indices << vertex + 1 + offset;
            qDebug() << vertex - (s_verticesPerRing - 1) << " <-> " << vertex + 1 + offset;

            m_indices << 56789;
            qDebug() << "Restart";

            countVertexOnRing = 0;
            ring = (ring + 1) % 2;
        }
    }


    for (int i = 0; i < m_indices.size()-2; ++i)
    {
        int index = m_indices.at(i);
        int index1 = m_indices.at(i+1);
        int index2 = m_indices.at(i+2);
        if (index != 56789 && index1 != 56789 && index2 != 56789)
        {
            glm::vec3 a = m_vertices.at(index1) - m_vertices.at(index);
            glm::vec3 b = m_vertices.at(index1) - m_vertices.at(index2);

            m_normals << glm::cross(a, b);
            glm::vec3 bla = glm::cross(a, b);
            //qDebug() << bla.x << " " << bla.y << " " << bla.z;
        }
        else if (index != 56789 && index1 != 56789)
        {
            glm::vec3 a = m_vertices.at(index1) - m_vertices.at(index);
            glm::vec3 b = m_vertices.at(index1) - m_vertices.at(m_indices.at(i+2-s_verticesPerRing));

            m_normals << glm::cross(a, b);
            //qDebug() << "Kante";
            glm::vec3 bla = glm::cross(a, b);
            //qDebug() << bla.x << " " << bla.y << " " << bla.z;
            i += 3;
        }


    }
}

void CaveDrawable::writeVertexList(int numRings)
{
    for (int i = 0 + m_activeRingPosition; i < numRings / 2.0f + m_activeRingPosition; i++) {
        for (glm::vec3 v : dummyArray){
            m_vertices << v + glm::vec3(0.0f, 0.0f, (i) * m_ringZStride);
            glm::vec3 a = v + glm::vec3(0.0f, 0.0f, (i) * m_ringZStride);
            qDebug() << a.x << " " << a.y << " " << a.z;
        }

        for (glm::vec3 v : dummyArrayOffset){
            m_vertices << v + glm::vec3(0.0f, 0.0f, (i + 0.5f) * m_ringZStride);
            glm::vec3 a = v + glm::vec3(0.0f, 0.0f, (i + 0.5f) * m_ringZStride);
            qDebug() << a.x << " " << a.y << " " << a.z;
        }

    }
}

void CaveDrawable::draw()
{
    if (m_vao == nullptr)
        return;
    m_vao->bind();
    int size = s_verticesPerRing * s_numRings * 3;
    m_vao->drawElements(GL_TRIANGLE_STRIP, size, GL_UNSIGNED_INT, nullptr);
    m_vao->unbind();
}

void CaveDrawable::update(glm::vec3 camPosition)
{
    if (camPosition.z < m_activeRingPosition * m_ringZStride)
    {
        for (int i = 0; i < s_verticesPerRing*2; i++)
            m_vertices.erase(m_vertices.begin() + 0);
        m_activeRingPosition += 1;
        writeVertexList(2);
    }
}