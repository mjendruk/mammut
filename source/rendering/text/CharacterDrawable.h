#pragma once

#include <glm/glm.hpp>

#include <glow/Array.h>
#include <glow/ref_ptr.h>

#include <rendering/world/IDrawable.h>

namespace glow
{
    class VertexArrayObject;
    class Buffer;
}

class CharacterDrawable : public IDrawable
{
public:
    CharacterDrawable();
    virtual ~CharacterDrawable();

    void initialize() override;
    void draw() override;

protected:
    static const glow::Vec2Array triangleStrip();
    
protected:
    glow::ref_ptr<glow::VertexArrayObject> m_vao;
    glow::ref_ptr<glow::Buffer> m_buffer;

};
