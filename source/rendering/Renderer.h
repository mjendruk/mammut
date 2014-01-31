#pragma once

namespace glow
{
    class FrameBufferObject;
}

class Renderer
{
public:
    virtual ~Renderer();

    virtual void resize(int width, int height) = 0;
    virtual void render(glow::FrameBufferObject * fbo, float devicePixelRatio) = 0;

};
