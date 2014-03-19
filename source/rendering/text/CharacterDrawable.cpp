#include "CharacterDrawable.h"

#include <cassert>

#include <glow/VertexArrayObject.h>
#include <glow/VertexAttributeBinding.h>
#include <glow/Buffer.h>

CharacterDrawable::CharacterDrawable()
{
    initialize();
}

CharacterDrawable::~CharacterDrawable()
{
}

const std::vector<glm::vec2> CharacterDrawable::triangleStrip()
{
    return {
        glm::vec2(0.0f, 0.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(1.0f, 1.0f),
        glm::vec2(0.0f, 1.0f)
    };
}

void CharacterDrawable::initialize()
{
    m_vao = new glow::VertexArrayObject();
    
    m_vao->bind();
    
    m_vertexBuffer = new glow::Buffer(GL_ARRAY_BUFFER);
    m_vertexBuffer->setData(triangleStrip());
    
    m_vertexTransformBuffer = new glow::Buffer(GL_ARRAY_BUFFER);
    m_textureCoordTransformBuffer = new glow::Buffer(GL_ARRAY_BUFFER);
    
    glow::VertexAttributeBinding * vertices = m_vao->binding(0);
    vertices->setAttribute(0);
    vertices->setBuffer(m_vertexBuffer, 0, sizeof(glm::vec2));
    vertices->setFormat(2, GL_FLOAT);
    m_vao->enable(0);
    
    const int vertexTransformLocation = 1;
    for (int i = 0; i < 4; ++i)
    {
        glow::VertexAttributeBinding * vertexTransforms = m_vao->binding(vertexTransformLocation + i);
        vertexTransforms->setAttribute(vertexTransformLocation + i);
        vertexTransforms->setBuffer(m_vertexTransformBuffer, 0, sizeof(glm::mat4));
        vertexTransforms->setFormat(4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * i);
        glVertexAttribDivisor(vertexTransformLocation + i, 1);
        m_vao->enable(vertexTransformLocation + i);
    }
    
    const int textureCoordTransformLocation = 5;
    for (int i = 0; i < 4; ++i)
    {
        glow::VertexAttributeBinding * vertexTransforms = m_vao->binding(textureCoordTransformLocation + i);
        vertexTransforms->setAttribute(textureCoordTransformLocation + i);
        vertexTransforms->setBuffer(m_textureCoordTransformBuffer, 0, sizeof(glm::mat4));
        vertexTransforms->setFormat(4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * i);
        glVertexAttribDivisor(textureCoordTransformLocation + i, 1);
        m_vao->enable(textureCoordTransformLocation + i);
    }
    
    m_vao->unbind();
}

void CharacterDrawable::draw(unsigned int instancesCount,
    const QVector<glm::mat4> & vertexTransforms,
    const QVector<glm::mat4> & textureCoordTransforms)
{
    assert(vertexTransforms.size() >= int(instancesCount));
    assert(textureCoordTransforms.size() >= int(instancesCount));
    
    m_vertexTransformBuffer->setData(instancesCount * sizeof(glm::mat4),
                                     vertexTransforms.data(),
                                     GL_DYNAMIC_DRAW);
    
    m_textureCoordTransformBuffer->setData(instancesCount * sizeof(glm::mat4),
                                           textureCoordTransforms.data(),
                                           GL_DYNAMIC_DRAW);
    
    m_vao->bind();
    m_vao->drawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, instancesCount);
    m_vao->unbind();
}
