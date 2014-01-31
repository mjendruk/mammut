#pragma once

#include <glm/glm.hpp>

#include <glow/Array.h>
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
    static const glow::Vec2Array triangleStrip();
    
protected:
    glow::ref_ptr<glow::VertexArrayObject> m_vao;
    glow::ref_ptr<glow::Buffer> m_buffer;

};
