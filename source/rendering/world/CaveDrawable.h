#pragma once

#include <memory>

#include <QVector>

#include <glow/ref_ptr.h>
#include <glow/Buffer.h>
#include <glow/VertexArrayObject.h>
#include <glow/Array.h>

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
    glow::Array<glm::vec3> m_vertices;
    glow::Array<glm::vec3> m_duplicatedVertices;
    glow::Array<glm::vec3> m_normals;
    glow::Array<unsigned int> m_indices;
    glow::VertexArrayObject * m_vao;
    glow::Buffer * m_vertexBuffer;
    glow::Buffer * m_normalBuffer;
    
    glow::Array<glm::vec3> dummyArray;
    glow::Array<glm::vec3> dummyArrayOffset;

};
