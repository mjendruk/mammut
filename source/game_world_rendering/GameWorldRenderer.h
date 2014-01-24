#pragma once

#include <QObject>
#include <QTime>

#include <glow/ref_ptr.h>

#include <Renderer.h>

#include "CuboidDrawable.h"
#include "CaveDrawable.h"
#include "Painter.h"
#include "CavePainter.h"
#include "RenderCamera.h"
#include "HUD.h"

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
class GameMechanics;
class SSAO;

class GameWorldRenderer : public Renderer
{
public:
    GameWorldRenderer(const GameMechanics * gameMechanics);
    virtual ~GameWorldRenderer();

    void initialize() override;
    void resize(int width, int height) override;
    void render(float devicePixelRatio) override;

    int fps() const;

    void setGameMechanics(const GameMechanics * mechanics);

protected:
    void initializeGBuffer();

protected:
    void updateFPS();

    static const float nearPlane;
    static const float farPlane;
    
    Painter m_painter;
    CavePainter m_cavePainter;
    RenderCamera m_camera;
    HUD m_hud;

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

    QTime m_lastFrame;
    float m_avgTimeSinceLastFrame;
    
    const GameMechanics * m_gameMechanics;
};
