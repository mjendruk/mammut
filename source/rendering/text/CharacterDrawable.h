#pragma once

#include <vector>

#include <QVector>

#include <glm/glm.hpp>

#include <glow/ref_ptr.h>

namespace glow
{
    class VertexArrayObject;
    class Buffer;
}

class CharacterDrawable
{
public:
    CharacterDrawable();
    virtual ~CharacterDrawable();

    void draw(unsigned int instancesCount,
              const QVector<glm::mat4> & vertexTransforms,
              const QVector<glm::mat4> & textureCoordTransforms);

protected:
    void initialize();
    static const std::vector<glm::vec2> triangleStrip();
    
protected:
    glow::ref_ptr<glow::VertexArrayObject> m_vao;
    glow::ref_ptr<glow::Buffer> m_vertexBuffer;
    glow::ref_ptr<glow::Buffer> m_vertexTransformBuffer;
    glow::ref_ptr<glow::Buffer> m_textureCoordTransformBuffer;

};
