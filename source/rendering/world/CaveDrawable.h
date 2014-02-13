#pragma once

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include <glow/ref_ptr.h>
#include <glow/Buffer.h>
#include <glow/VertexArrayObject.h>

#include "DrawableInterface.h"

class CaveDrawable : public DrawableInterface
{
public:
    CaveDrawable();
    ~CaveDrawable();

    void draw() override;
    void update(glm::vec3 camPosition);

protected:
    void initialize() override;
    void initializeIndices();
    void addRings(int numRings);
    void buildDuplicatedVertices();
    void buildNormals();

    static const int s_verticesPerRing;
    static const float s_radius;
    static const glm::vec3 s_maxShift;
    static const int s_numRings;

    float m_ringZStride;
    float m_activeRingPosition; //ring count
    std::vector<glm::vec3> m_vertices;
    std::vector<glm::vec3> m_duplicatedVertices;
    std::vector<glm::vec3> m_normals;
    std::vector<unsigned int> m_indices;
    glow::VertexArrayObject * m_vao;
    glow::Buffer * m_vertexBuffer;
    glow::Buffer * m_normalBuffer;
    
    std::vector<glm::vec3> dummyArray;
    std::vector<glm::vec3> dummyArrayOffset;

};
