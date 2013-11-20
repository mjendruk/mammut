
#pragma once

#include <glow/ref_ptr.h>
#include <glow/Array.h>

namespace glow
{
    class VertexArrayObject;
    class Buffer;
}

class Drawable
{
public:
    Drawable();
    ~Drawable();

    void initialize();
    void draw();

protected:
    static glow::Vec3Array vertices();
    static glow::Array<glm::lowp_uvec3> indices();

protected:
    glow::ref_ptr<glow::VertexArrayObject> m_vao;
    glow::ref_ptr<glow::Buffer> m_vertices;
    glow::ref_ptr<glow::Buffer> m_indices;

};
