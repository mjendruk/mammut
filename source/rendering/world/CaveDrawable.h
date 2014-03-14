#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "DrawableInterface.h"

namespace glow
{
    class Buffer;
    class VertexArrayObject;
}

class CaveDrawable : public DrawableInterface
{
public:
    CaveDrawable();
    ~CaveDrawable();

    void draw() override;
    void update(glm::vec3 camPosition);

protected:
    void initialize() override;
    void initializeDummyArrays();
    void initializeIndices();

    glm::vec3 getRandomOffset();
    void addTwoRings();
    void rebuildGPUData();
    void buildDuplicatedVertices();
    void buildNormals();

protected:
    static const int s_verticesPerRing;
    static const glm::vec3 s_maxShift;
    static const float s_ringZStride;
    static const int s_numRings;
    static const int s_numSafetyMarginRings;

    int m_lastRingIndex;
    std::vector<glm::vec3> m_vertices;
    std::vector<glm::vec3> m_duplicatedVertices;
    std::vector<glm::vec3> m_normals;
    std::vector<unsigned int> m_indices;
    glow::VertexArrayObject * m_vao;
    glow::Buffer * m_vertexBuffer;
    glow::Buffer * m_normalBuffer;
    
    std::vector<glm::vec3> m_dummyArray;
    std::vector<glm::vec3> m_dummyArrayOffset;

};
