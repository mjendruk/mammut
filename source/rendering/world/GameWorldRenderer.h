#pragma once

#include <QObject>
#include <QTime>

#include <glow/ref_ptr.h>

#include <rendering/Renderer.h>

#include "CuboidDrawable.h"
#include "CaveDrawable.h"
#include "Painter.h"
#include "CavePainter.h"
#include "RenderCamera.h"
#include "HUD.h"
#include "SimplePostProcPass.h"
#include "MotionBlurPostProc.h"
#include "SSAOPostProc.h"

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

class GameWorldRenderer : public Renderer
{
public:
    GameWorldRenderer();
    virtual ~GameWorldRenderer();

    void resize(int width, int height) override;
    void render(glow::FrameBufferObject * fbo, float devicePixelRatio) override;

    int fps() const;

    void setGameMechanics(const GameMechanics * mechanics);

protected:
    void initialize();
    void initializeGBuffers();
    void initializePostProcPasses();

    void updatePainters();

    void updateFPS();

protected:
    static const float nearPlane;
    static const float farPlane;
    
    Painter m_painter;
    CavePainter m_cavePainter;
    RenderCamera m_camera;
    HUD m_hud;

    CuboidDrawable m_cuboidDrawable;
    CaveDrawable m_caveDrawable;

    glow::ref_ptr<glow::FrameBufferObject> m_gBufferFBO;
    glow::ref_ptr<glow::FrameBufferObject> m_ssaoFBO;
    glow::ref_ptr<glow::FrameBufferObject> m_motionBlurFBO;

    glow::ref_ptr<glow::Texture> m_gBufferDepth;
    glow::ref_ptr<glow::Texture> m_gBufferNormals;
    glow::ref_ptr<glow::Texture> m_gBufferColor;
    glow::ref_ptr<glow::Texture> m_gBufferVelocity;

    glow::ref_ptr<glow::Texture> m_ssaoOutput;
    glow::ref_ptr<glow::Texture> m_motionBlurOutput;
    
    SimplePostProcPass m_quadPass;
    MotionBlurPostProc m_motionBlurPostProc;
    SSAOPostProc m_ssaoPostProc;

    glm::mat4 m_previousViewProjection;
    
    QTime m_lastFrame;
    float m_avgTimeSinceLastFrame;
    
    const GameMechanics * m_gameMechanics;
};
