#pragma once

#include <memory>

#include <QVector>

#include <glow/ref_ptr.h>
#include <glow/Buffer.h>
#include <glow/VertexArrayObject.h>
#include <glow/Array.h>

#include "IDrawable.h"

class CaveDrawable : public IDrawable
{
public:
    CaveDrawable();
    ~CaveDrawable();

    void initialize() override;
    void draw() override;

protected:
    void initializeData();


    static const int s_verticesPerRing;
    static const float s_radius;
    static const glm::vec3 s_maxShift;
    static const int s_numRings;

    int m_numIndices;
    glow::Array<glm::vec3> m_vertices;
    glow::Array<glm::vec3> m_normals;
    glow::Array<unsigned int> m_indices;
    glow::VertexArrayObject * m_vao;
    glow::Buffer * m_vertexBuffer;
    glow::Buffer * m_normalBuffer;
    glow::Buffer * m_indexBuffer;


};
