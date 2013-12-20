#pragma once

#include <QObject>

#include <glow/ref_ptr.h>

#include "CuboidDrawable.h"
#include "CaveDrawable.h"
#include "Painter.h"
#include "CavePainter.h"
#include "RenderCamera.h"

namespace glow
{
    class FrameBufferObject;
    class Texture;
    class Program;
}

namespace glowutils
{
    class ScreenAlignedQuad;
}

class Canvas;
class GameLogic;
class SSAO;

class Renderer
{
public:
    Renderer(GameLogic & gameLogic, Canvas & canvas);
    ~Renderer();

    void initialize();
    void paint();
    void resize(int width, int height);

protected:
    void initializeGBuffer();

protected:
    static const float nearPlane;
    static const float farPlane;
    
    CavePainter m_cavePainter;
    GameLogic & m_gameLogic;
    Canvas & m_canvas;
    
    Painter m_painter;
    RenderCamera m_camera;

    CuboidDrawable m_cuboidDrawable;
    CaveDrawable m_caveDrawable;

    glow::ref_ptr<glow::Program> m_DepthProgram;
    glow::ref_ptr<glow::FrameBufferObject> m_gBufferFBO;
    glow::ref_ptr<glow::Texture> m_gBufferDepth;
    glow::ref_ptr<glow::Texture> m_gBufferNormals;
    glow::ref_ptr<glow::Texture> m_gBufferColor;
    glow::ref_ptr<glow::Texture> m_ssaoOutput;
    glow::ref_ptr<glowutils::ScreenAlignedQuad> m_quad;
    glow::ref_ptr<SSAO> m_ssao;
};
