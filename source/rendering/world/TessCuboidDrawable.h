#pragma once

#include "DrawableInterface.h"

#include <glow/ref_ptr.h>

class Cuboid;

namespace glow
{
    class Buffer;
    class VertexArrayObject;
}

class TessCuboidDrawable : public DrawableInterface
{
public:
    TessCuboidDrawable(const Cuboid * cuboid);
    ~TessCuboidDrawable();

    void draw() override;

    const Cuboid * cuboid();

protected:
    void initialize() override;

protected:
    const Cuboid * m_cuboid;
    glow::ref_ptr<glow::VertexArrayObject> m_vao;
    glow::ref_ptr<glow::Buffer> m_vertexBuffer;
    glow::ref_ptr<glow::Buffer> m_normalBuffer;
};