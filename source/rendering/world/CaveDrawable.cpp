#include "CaveDrawable.h"

#include <cmath>

#include <glm/gtx/random.hpp>

#include <glow/VertexArrayObject.h>
#include <glow/Buffer.h>
#include <glow/VertexAttributeBinding.h>

#include <logic/world/Cave.h>


const int CaveDrawable::s_verticesPerRing = 20;
const glm::vec3 CaveDrawable::s_maxShift = glm::vec3(15.0f, 15.0f, 20.0f);
const float CaveDrawable::s_ringZStride = -50.0f;
const int CaveDrawable::s_numRings = 35;
const int CaveDrawable::s_numSafetyMarginRings = 7;

CaveDrawable::CaveDrawable(const Cave & cave)
:   m_lastRingIndex(-s_numSafetyMarginRings)
,   m_cave(cave)
,   m_vao(nullptr)
,   m_vertexBuffer(nullptr)
,   m_normalBuffer(nullptr)
{

    initialize();
}

CaveDrawable::~CaveDrawable()
{
}

void CaveDrawable::initialize()
{
    initializeDummyArrays();
    initializeIndices();
    for (int i = 0; i < s_numRings / 2; i++)
        addTwoRings();

    const GLuint vertexAttribLocation = 0;
    const GLuint normalAttribLocation = 1;

    m_vao = new glow::VertexArrayObject();

    m_vertexBuffer = new glow::Buffer(GL_ARRAY_BUFFER);
    m_normalBuffer = new glow::Buffer(GL_ARRAY_BUFFER);

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

    rebuildGPUData();
}

void CaveDrawable::initializeDummyArrays()
{
    for (int i = 0; i < s_verticesPerRing; i++) {
        float angle = (float(M_PI) * 2 / s_verticesPerRing) * i;
        m_dummyArray.push_back(glm::vec3(cos(angle), sin(angle), 0.0) * Cave::s_caveRadius);

        float angleOffset = (float(M_PI) * 2 / s_verticesPerRing) * (i + 0.5f);
        m_dummyArrayOffset.push_back(glm::vec3(cos(angleOffset), sin(angleOffset), 0.0) * Cave::s_caveRadius);
    }
}

void CaveDrawable::initializeIndices()
{
    for (int ring = 0; ring < s_numRings / 2; ring++) {
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
    int size = s_verticesPerRing * s_numRings * 3;
    m_vao->drawArrays(GL_TRIANGLES, 0, size);
    m_vao->unbind();
}

void CaveDrawable::update(glm::vec3 camPosition)
{
    while (camPosition.z - m_cave.zShift() < (m_lastRingIndex - s_numRings + s_numSafetyMarginRings) * s_ringZStride) {
        m_vertices.erase(m_vertices.begin(), m_vertices.begin() + 2 * s_verticesPerRing);
        addTwoRings();
    }

    rebuildGPUData();
}

glm::vec3 CaveDrawable::getRandomOffset()
{
    return glm::vec3(
        glm::linearRand(-s_maxShift.x, s_maxShift.x),
        glm::linearRand(-s_maxShift.y, s_maxShift.y),
        glm::linearRand(-s_maxShift.z, s_maxShift.z));
}

void CaveDrawable::addTwoRings()
{
    float firstRingPosition = (m_lastRingIndex + 1) * s_ringZStride;
    float secondRingPosition = (m_lastRingIndex + 2) * s_ringZStride;

    for (const glm::vec3 & v : m_dummyArray)
        m_vertices.push_back(v + glm::vec3(0.0f, 0.0f, firstRingPosition) + getRandomOffset());

    for (const glm::vec3 & v : m_dummyArrayOffset)
        m_vertices.push_back(v + glm::vec3(0.0f, 0.0f, secondRingPosition) + getRandomOffset());

    m_lastRingIndex += 2;
}

void CaveDrawable::rebuildGPUData()
{
    buildGPUVertices();
    buildNormals();

    m_vertexBuffer->setData(m_GPUVertices);
    m_normalBuffer->setData(m_normals);
}

void CaveDrawable::buildGPUVertices()
{
    glm::vec3 zShiftVector = glm::vec3(0.0f, 0.0f, m_cave.zShift());
    m_GPUVertices.clear();
    for (int i : m_indices) {
        m_GPUVertices.push_back(m_vertices[i] + zShiftVector);
    }
}

void CaveDrawable::buildNormals()
{
    m_normals.clear();
    for (int i = 0; i < m_GPUVertices.size(); i += 3) {

        glm::vec3 ab = m_GPUVertices.at(i + 1) - m_GPUVertices.at(i);
        glm::vec3 ac = m_GPUVertices.at(i + 2) - m_GPUVertices.at(i);

        glm::vec3 normal = glm::normalize(glm::cross(ab, ac));

        m_normals.push_back(normal);
        m_normals.push_back(normal);
        m_normals.push_back(normal);
    }
}
