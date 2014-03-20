#include "GameWorldRenderer.h"

#include <cassert>

#include <glm/gtx/transform.hpp>

#include <QMap>

#include <glow/global.h>
#include <glow/Texture.h>
#include <glow/Program.h>
#include <glow/FrameBufferobject.h>
#include <glowutils/File.h>
#include <glowutils/ScreenAlignedQuad.h>

#include <logic/world/Cuboid.h>
#include <logic/world/Mammut.h>
#include <logic/world/GameMechanics.h>
#include <logic/world/GameCamera.h>

#include "Util.h"
#include "PerfCounter.h"

const float GameWorldRenderer::s_nearPlane = 0.01f;
const float GameWorldRenderer::s_farPlane = 700.0f;

GameWorldRenderer::GameWorldRenderer()
:   m_hud(m_camera, *this)
,   m_caveDrawable(nullptr)
,   m_lastFrame(QTime::currentTime())
,   m_avgTimeSinceLastFrame(0.0f)
,   m_gameMechanics(nullptr)
{
    glow::createNamedString("/Fxaa3_11.h", new glowutils::File("data/shaders/Fxaa3_11.h"));
    m_FxaaPass.reset(new SimplePostProcPass("data/shaders/fxaa.frag", GL_RGB8));
    initialize();
}

GameWorldRenderer::~GameWorldRenderer()
{
}

void GameWorldRenderer::render(glow::FrameBufferObject * fbo, float devicePixelRatio)
{
    assert(m_gameMechanics != nullptr);

    m_previousViewProjection *= glm::translate(0.0f, 0.0f, -m_gameMechanics->lastZShift());
    
    glViewport(0, 0, m_camera.viewport().x, m_camera.viewport().y);
   
    m_camera.update(m_gameMechanics->camera());
    m_caveDrawable->update(m_camera.eye());

    updateFPS();
    updatePainters();
    
    // render
    drawGeometry();
    applyPostproc(fbo, devicePixelRatio);
    
    //paint HUD over
    fbo->bind();
    int velocity = int(-m_gameMechanics->mammut().velocity().z);
    m_hud.paint(velocity, m_gameMechanics->score());
    fbo->unbind();

    // update previous view projection matrix for next frame
    m_previousViewProjection = m_camera.viewProjection();
}

void GameWorldRenderer::drawGeometry()
{
    PerfCounter::beginGL("geom");
    m_gBufferFBO->bind();
    
    glEnable(GL_CULL_FACE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // set the default view space depth to - s_farPlane
    m_gBufferFBO->clearBuffer(GL_COLOR, 0, glm::vec4(0.0f, 0.0f, 0.0f, -s_farPlane));
    
    
    m_gameMechanics->forEachCuboid([this](const Cuboid * cuboid) {
        // modelMatrix and previous modelMatrix are the same until they will begin to move (e.g. destruction) [motionBlur]
        m_painter.paint(m_cuboidDrawable, cuboid->modelTransform(), cuboid->modelTransform(), cuboid->containsBoost());
    });
    
    // cave does not move at the moment, so model and prevModel are the same [motionBlur]
    m_cavePainter.paint(*m_caveDrawable, glm::mat4(), glm::mat4());
    m_gBufferFBO->unbind();
    PerfCounter::endGL("geom");
}

void GameWorldRenderer::applyPostproc(glow::FrameBufferObject * fbo, float devicePixelRatio)
{
    glDisable(GL_DEPTH_TEST);

    PerfCounter::beginGL("ssao");
    m_ssaoPass.setProjectionUniform(m_camera.projection());
    m_ssaoPass.setInverseProjectionUniform(m_camera.projectionInverted());
    m_ssaoPass.setFarPlaneUniform(s_farPlane);

    m_ssaoPass.apply();
    PerfCounter::endGL("ssao");

    PerfCounter::beginGL("mb");
    m_motionBlurPass.setFPSUniform(fps() / 60.f);
    m_motionBlurPass.apply();
    PerfCounter::endGL("mb");

    PerfCounter::beginGL("fxaa");
    m_FxaaPass->apply();
    PerfCounter::endGL("fxaa");

    PerfCounter::beginGL("blit");
    glViewport(0, 0,
        m_camera.viewport().x * devicePixelRatio,
        m_camera.viewport().y * devicePixelRatio);

    fbo->bind();
    m_renderOnScreenQuad->draw();
    fbo->unbind();
    PerfCounter::endGL("blit");

    glEnable(GL_DEPTH_TEST);
}

void GameWorldRenderer::initialize()
{
    m_camera.setFovy(80.0);
    m_camera.setZNear(s_nearPlane);
    m_camera.setZFar(s_farPlane);

    initializeGBuffers();
    initializePostProcPasses();
}

void GameWorldRenderer::initializeGBuffers()
{
    m_gBufferFBO = new glow::FrameBufferObject();

    m_gBufferColor = Util::create2DTexture(GL_NEAREST, GL_CLAMP_TO_EDGE);
    m_gBufferNormalDepth = Util::create2DTexture(GL_NEAREST, GL_CLAMP_TO_EDGE);
    m_gBufferDepth = Util::create2DTexture(GL_NEAREST, GL_CLAMP_TO_EDGE);
    m_gBufferVelocity = Util::create2DTexture(GL_NEAREST, GL_CLAMP_TO_EDGE);

    m_gBufferFBO->attachTexture2D(GL_COLOR_ATTACHMENT0, m_gBufferNormalDepth);
    m_gBufferFBO->attachTexture2D(GL_COLOR_ATTACHMENT1, m_gBufferColor);
    m_gBufferFBO->attachTexture2D(GL_COLOR_ATTACHMENT2, m_gBufferVelocity);
    m_gBufferFBO->attachTexture2D(GL_DEPTH_ATTACHMENT, m_gBufferDepth);

    m_gBufferFBO->setDrawBuffers({ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 });
}


void GameWorldRenderer::initializePostProcPasses()
{
    // SSAO
    m_ssaoOutput = m_ssaoPass.outputTexture();
    m_ssaoPass.setInputTextures({ { "color", m_gBufferColor },
                                  { "normal_depth", m_gBufferNormalDepth } });

    // Motion Blur
    m_motionBlurOutput = m_motionBlurPass.outputTexture();
    m_motionBlurPass.setInputTextures({ { "color", m_ssaoOutput },
                                        { "depth", m_gBufferDepth },
                                        { "velocity", m_gBufferVelocity } });
    
    // FXAA
    m_FxaaOutput = m_FxaaPass->outputTexture();
    m_FxaaPass->setInputTextures({ { "buf0", m_motionBlurOutput } });

    // set texture that will be rendered on screen
    m_renderOnScreenQuad = new glowutils::ScreenAlignedQuad(m_FxaaOutput);
}

void GameWorldRenderer::resize(int width, int height)
{
    m_camera.setViewport(glm::ivec2(width, height));
    
    m_gBufferNormalDepth->image2D(0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
    m_gBufferColor->image2D(0, GL_RGB8, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
    m_gBufferVelocity->image2D(0, GL_RG16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
    m_gBufferDepth->image2D(0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    m_ssaoPass.resize(width, height);
    m_motionBlurPass.resize(width, height);
    m_FxaaPass->resize(width, height);
}

void GameWorldRenderer::updateFPS()
{
    float weightRatio = 0.9f;
    m_avgTimeSinceLastFrame = weightRatio * m_avgTimeSinceLastFrame + (1.0 - weightRatio) * m_lastFrame.elapsed();
    m_lastFrame = QTime::currentTime();
}

void GameWorldRenderer::updatePainters()
{
    m_painter.setViewProjectionUniforms(m_camera.viewProjection(), m_previousViewProjection);
    m_painter.setViewUniform(m_camera.view());

    m_cavePainter.setViewProjectionUniforms(m_camera.viewProjection(), m_previousViewProjection);
    m_cavePainter.setViewUniform(m_camera.view());
}

int GameWorldRenderer::fps() const
{
    return (int) (1000.0 / m_avgTimeSinceLastFrame + 0.5);
}

void GameWorldRenderer::setGameMechanics(const GameMechanics * mechanics)
{
    assert(mechanics != nullptr);
    m_gameMechanics = mechanics;
    m_caveDrawable.reset(new CaveDrawable(mechanics->cave()));
}

