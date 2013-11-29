#pragma once

#include <QObject>

#include <glow/ref_ptr.h>

#include "Drawable.h"
#include "Painter.h"
#include "RenderCamera.h"

namespace glow
{
    class FrameBufferObject;
    class Texture;
    class Program;
    class ScreenAlignedQuad;
}

class Canvas;
class GameLogic;
class SSAO;

class Renderer
{
public:
    Renderer(GameLogic & gameLogic);
    ~Renderer();

    void render();
    void resize(int width, int height);

    void registerKeyHandler(QObject & keyHandler);

protected:
    void initialize();
    void initializeGBuffer();

protected:
    Canvas * m_canvas;
    Painter m_painter;
    GameLogic & m_gameLogic;
    bool m_initialized;

    RenderCamera m_camera;

    Drawable m_cuboidDrawable;

    glow::ref_ptr<glow::Program> m_DepthProgram;
    glow::ref_ptr<glow::FrameBufferObject> m_gBufferFBO;
    glow::ref_ptr<glow::Texture> m_gBufferDepth;
    glow::ref_ptr<glow::Texture> m_gBufferNormals;
    glow::ref_ptr<glow::Texture> m_gBufferColor;
    glow::ref_ptr<glow::Texture> m_ssaoOutput;
    glow::ref_ptr<glow::ScreenAlignedQuad> m_quad;
    glow::ref_ptr<SSAO> m_ssao;
};
