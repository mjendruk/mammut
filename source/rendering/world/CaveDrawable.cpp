#include "CaveDrawable.h"

#include <glm/gtx/random.hpp>

#include <glow/VertexArrayObject.h>
#include <glow/Buffer.h>
#include <glow/VertexAttributeBinding.h>


const int CaveDrawable::s_verticesPerRing = 20;
const float CaveDrawable::s_radius = 200.f;
const glm::vec3 CaveDrawable::s_maxShift = glm::vec3(0.f); //glm::vec3(1.f, 1.f, 1.f);
const int CaveDrawable::s_numRings = 30;

CaveDrawable::CaveDrawable()
:   m_vertexBuffer(nullptr)
,   m_normalBuffer(nullptr)
,   m_vao(nullptr)
,   m_ringZStride(-100.f)
,   m_activeRingPosition(0.f)
{
    for (int i = 0; i < s_verticesPerRing; i++) {
        float angle = (3.14159265359f * 2 / s_verticesPerRing) * i;
        dummyArray.push_back(glm::vec3(cos(angle), sin(angle), 0.0) * s_radius);

        float angleOffset = (3.14159265359f * 2 / s_verticesPerRing) * (i + 0.5f);
        dummyArrayOffset.push_back(glm::vec3(cos(angleOffset), sin(angleOffset), 0.0) * s_radius);
    }

    initialize();
}

CaveDrawable::~CaveDrawable()
{
}

void CaveDrawable::initialize()
{
    initializeIndices();
    addRings(s_numRings);

    const GLuint vertexAttribLocation = 0;
    const GLuint normalAttribLocation = 1;

    m_vao = new glow::VertexArrayObject();

    m_vertexBuffer = new glow::Buffer(GL_ARRAY_BUFFER);
    m_normalBuffer = new glow::Buffer(GL_ARRAY_BUFFER);

    m_vertexBuffer->setData(m_duplicatedVertices, GL_DYNAMIC_DRAW);
    m_normalBuffer->setData(m_normals, GL_DYNAMIC_DRAW);

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

    m_vao->unbind();
}

void CaveDrawable::initializeIndices()
{
    int ring = 0;

    for (int ring = 0; ring < s_numRings; ring++) {
        for (int vertex = 0; vertex < s_verticesPerRing; vertex++) {
            int index = ring * s_verticesPerRing;
            index += vertex;
            
            if (ring % 2 == 0) {
                m_indices.push_back(index + s_verticesPerRing);
                m_indices.push_back(index + ((vertex == 0) ? s_verticesPerRing * 2 - 1 : s_verticesPerRing - 1));
                m_indices.push_back(index);

                m_indices.push_back(index + ((vertex == (s_verticesPerRing - 1)) ? -s_verticesPerRing + 1 : 1));
                m_indices.push_back(index + s_verticesPerRing);
                m_indices.push_back(index);
            }
            else {
                m_indices.push_back(index + ((vertex == (s_verticesPerRing - 1)) ? 1 : s_verticesPerRing + 1));
                m_indices.push_back(index + s_verticesPerRing);
                m_indices.push_back(index);

                m_indices.push_back(index + ((vertex == (s_verticesPerRing - 1)) ? -s_verticesPerRing + 1 : 1));
                m_indices.push_back(index + ((vertex == (s_verticesPerRing - 1)) ? 1 : s_verticesPerRing + 1));
                m_indices.push_back(index);
            }
        }
    }
}

void CaveDrawable::draw()
{
    if (m_vao == nullptr)
        return;
    m_vao->bind();
    int size = s_verticesPerRing * s_numRings*2 * 3;
    m_vao->drawArrays(GL_TRIANGLES, 0, size);
    m_vao->unbind();
}

void CaveDrawable::update(glm::vec3 camPosition)
{
    int replaceRings = 2;
    while (camPosition.z < m_activeRingPosition * m_ringZStride + m_ringZStride)
    {
        if (replaceRings > 0)
        {
            for (int i = 0; i < replaceRings*s_verticesPerRing; i++)
                m_vertices.erase(m_vertices.begin() + 0);

            m_activeRingPosition += replaceRings / 2;

            addRings(replaceRings / 2);

            m_vertexBuffer->setData(m_duplicatedVertices);
            m_normalBuffer->setData(m_normals);
        }

    }
}

glm::vec3 getRandomOffset()
{
    return glm::vec3(
        glm::linearRand(0.0f, 20.0f),
        glm::linearRand(0.0f, 20.0f),
        glm::linearRand(0.0f, 40.0f));
}

void CaveDrawable::addRings(int numRings)
{       
    int size = m_vertices.size();
    for (int i = m_activeRingPosition + s_numRings - numRings; i < m_activeRingPosition + s_numRings; i++) {
        for (glm::vec3 v : dummyArray){
            m_vertices.push_back(v + glm::vec3(0.0f, 0.0f, (i)* m_ringZStride) + getRandomOffset());
        }

        for (glm::vec3 v : dummyArrayOffset){
            m_vertices.push_back(v + glm::vec3(0.0f, 0.0f, (i + 0.5f) * m_ringZStride) + getRandomOffset());
        }
    }
    buildDuplicatedVertices();
    buildNormals();
}

void CaveDrawable::buildDuplicatedVertices()
{
    m_duplicatedVertices.clear();
    for (int i : m_indices) {
        m_duplicatedVertices.push_back(m_vertices[i]);
    }
}

void CaveDrawable::buildNormals()
{
    m_normals.clear();
    for (int i = 0; i < m_duplicatedVertices.size(); i += 3) {

        glm::vec3 ab = m_duplicatedVertices.at(i + 1) - m_duplicatedVertices.at(i);
        glm::vec3 ac = m_duplicatedVertices.at(i + 2) - m_duplicatedVertices.at(i);

        glm::vec3 normal = glm::normalize(glm::cross(ab, ac));

        m_normals.push_back(normal);
        m_normals.push_back(normal);
        m_normals.push_back(normal);
    }
}
