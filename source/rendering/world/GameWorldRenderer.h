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
class SSAOPass;
class PostprocessingPass;


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
    void initializeGBuffer();

protected:
    void updateFPS();
    glow::ref_ptr<glow::Texture> create2DTexture();

    static const float nearPlane;
    static const float farPlane;
    static const float motionBlurRadius;
    static const float motionBlurS;
    
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
    glow::ref_ptr<glow::Texture> m_motionBlurTM;
    glow::ref_ptr<glow::Texture> m_motionBlurTMTemp;
    glow::ref_ptr<glow::Texture> m_motionBlurNM;
    glow::ref_ptr<glow::Texture> m_motionBlurNoise;
    glow::ref_ptr<glow::Texture> m_motionBlurOutput;
    
    glow::ref_ptr<SSAOPass> m_ssaoPass;
    glow::ref_ptr<PostprocessingPass> m_quadPass;


    glm::mat4 m_previousViewProjection;
    glm::mat4 m_previousMammutModel;
    
    QTime m_lastFrame;
    float m_avgTimeSinceLastFrame;
    
    const GameMechanics * m_gameMechanics;
};
