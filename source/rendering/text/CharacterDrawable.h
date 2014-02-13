#pragma once

#include <vector>

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

    void draw();

protected:
    void initialize();
    static const std::vector<glm::vec2> triangleStrip();
    
protected:
    glow::ref_ptr<glow::VertexArrayObject> m_vao;
    glow::ref_ptr<glow::Buffer> m_buffer;

};
