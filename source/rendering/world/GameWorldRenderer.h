#pragma once

#include <memory>
#include <vector>

#include <QTime>
#include <QList>

#include <glow/ref_ptr.h>

#include <rendering/Renderer.h>

#include <rendering/world/postprocessing/MotionBlurPass.h>
#include <rendering/world/postprocessing/SSAOPass.h>
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

    void drawGeometry();
    void applyPostproc(glow::FrameBufferObject * fbo, float devicePixelRatio);

protected:
    static const float s_nearPlane;
    static const float s_farPlane;
    
    Painter m_painter;
    CavePainter m_cavePainter;
    RenderCamera m_camera;
    HUD m_hud;

    CuboidDrawable m_cuboidDrawable;
    std::unique_ptr<CaveDrawable> m_caveDrawable;

    glow::ref_ptr<glow::FrameBufferObject> m_gBufferFBO;

    glow::ref_ptr<glow::Texture> m_gBufferDepth;
    glow::ref_ptr<glow::Texture> m_gBufferNormalDepth;
    glow::ref_ptr<glow::Texture> m_gBufferColor;
    glow::ref_ptr<glow::Texture> m_gBufferVelocity;

    glow::ref_ptr<glow::Texture> m_ssaoOutput;
    glow::ref_ptr<glow::Texture> m_motionBlurOutput;
    glow::ref_ptr<glow::Texture> m_FxaaOutput;
    
    SSAOPass m_ssaoPass;
    MotionBlurPass m_motionBlurPass;
    std::unique_ptr<SimplePostProcPass> m_FxaaPass;
    glow::ref_ptr<glowutils::ScreenAlignedQuad> m_renderOnScreenQuad;
    
    QList<glm::mat4> m_viewProjectionStack;
    std::vector<glm::vec3> m_particlePositions;
    glow::ref_ptr<glow::Buffer> m_particlesBuffer;
    glow::ref_ptr<glow::VertexArrayObject> m_particlesVbo;
    glow::ref_ptr<glow::Program> m_particlesProgram;

    glm::mat4 m_previousViewProjection;
    
    QTime m_lastFrame;
    float m_avgTimeSinceLastFrame;
    
    const GameMechanics * m_gameMechanics;
};
