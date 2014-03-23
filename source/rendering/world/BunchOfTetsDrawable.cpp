#include "BunchOfTetsDrawable.h"

#include <cassert>

#include <glow/Texture.h>
#include <glow/VertexArrayObject.h>
#include <glow/Buffer.h>
#include <glow/VertexAttributeBinding.h>

#include <Util.h>
#include "logic/world/tets/BunchOfTets.h"

BunchOfTetsDrawable::BunchOfTetsDrawable(const BunchOfTets & bunchOfTets)
:   m_bunchOfTets(bunchOfTets)
{
    initialize();
}

BunchOfTetsDrawable::~BunchOfTetsDrawable()
{
}

void BunchOfTetsDrawable::initialize()
{
    std::vector<glm::vec3> dummyTet;
    for (int i = 0; i < 3; i++)
        dummyTet.push_back(glm::vec3(0.0, 0.0, 0.0));

    glow::Buffer m_dummyVertexBuffer(GL_ARRAY_BUFFER);
    m_dummyVertexBuffer.setData(dummyTet, GL_STATIC_DRAW);


    m_vao = new glow::VertexArrayObject();
    m_vao->bind();

    auto vertexBinding = m_vao->binding(0);
    vertexBinding->setAttribute(0);
    vertexBinding->setBuffer(&m_dummyVertexBuffer, 0, sizeof(glm::vec3));
    vertexBinding->setFormat(3, GL_FLOAT, GL_FALSE);
    m_vao->enable(0);

    m_vao->unbind();

    m_vertexBuffer = new glow::Buffer(GL_TEXTURE_BUFFER);
    m_vertexBuffer->setData(0);
    m_vertexTexture = new glow::Texture(GL_TEXTURE_BUFFER);
    m_vertexTexture->texBuffer(GL_RGB32F, m_vertexBuffer);

    m_normalBuffer = new glow::Buffer(GL_TEXTURE_BUFFER);
    m_normalBuffer->setData(0);
    m_normalTexture = new glow::Texture(GL_TEXTURE_BUFFER);
    m_normalTexture->texBuffer(GL_RGB32F, m_normalBuffer);

    m_matrixBuffer = new glow::Buffer(GL_TEXTURE_BUFFER);
    m_matrixBuffer->setData(0);
    m_matrixTexture = new glow::Texture(GL_TEXTURE_BUFFER);
    m_matrixTexture->texBuffer(GL_RGBA32F, m_matrixBuffer);
}

void BunchOfTetsDrawable::paint()
{
    std::vector<glm::vec3> vertices;
    m_bunchOfTets.collectVertices(vertices);
    m_vertexBuffer->setData(vertices);

    std::vector<glm::vec3> normals;
    m_bunchOfTets.collectNormals(normals);
    m_normalBuffer->setData(normals);

    std::vector<glm::mat4> matrices;
    m_bunchOfTets.collectMatrices(matrices);
    m_matrixBuffer->setData(matrices);


    m_vertexTexture->bindActive(GL_TEXTURE0);
    m_normalTexture->bindActive(GL_TEXTURE1);
    m_matrixTexture->bindActive(GL_TEXTURE2);

    m_vao->bind();
    m_vao->drawArraysInstanced(GL_TRIANGLES, 0, 3, vertices.size() / 3);
    m_vao->unbind();

    m_matrixTexture->unbind(GL_TEXTURE2);
    m_normalTexture->unbind(GL_TEXTURE1);
    m_vertexTexture->unbind(GL_TEXTURE0);

}

const BunchOfTets * BunchOfTetsDrawable::bunch()
{
    return &m_bunchOfTets;
}
