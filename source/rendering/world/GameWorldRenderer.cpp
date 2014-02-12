#include "GameWorldRenderer.h"

#include <cassert>

#include <glm/gtx/transform.hpp>

#include <QMap>

#include "glow/Program.h"
#include "glow/FrameBufferobject.h"
#include "glowutils/File.h"
#include "glowutils/ScreenAlignedQuad.h"

#include <logic/world/Cuboid.h>
#include <logic/world/Mammut.h>
#include <logic/world/GameMechanics.h>
#include <logic/world/GameCamera.h>

#include "Painter.h"
#include "RenderCamera.h"
#include "FileAssociatedShader.h"
#include "SimplePostProcPass.h"
#include "MotionBlurPostProc.h"
#include "SSAOPostProc.h"
#include "Util.h"

const float GameWorldRenderer::nearPlane = 0.01f;
const float GameWorldRenderer::farPlane = 700.0f;

GameWorldRenderer::GameWorldRenderer()
:   m_hud(m_camera, *this)
,   m_lastFrame(QTime::currentTime())
,   m_gameMechanics(nullptr)
,   m_avgTimeSinceLastFrame(0.f)
{
    initialize();
}

GameWorldRenderer::~GameWorldRenderer()
{
}

void GameWorldRenderer::render(glow::FrameBufferObject * fbo, float devicePixelRatio)
{
    assert(m_gameMechanics != nullptr);
    
    glViewport(0, 0, m_camera.viewport().x, m_camera.viewport().y);
   
    m_camera.update(m_gameMechanics->camera());
    m_caveDrawable.update(m_camera.eye());

    updateFPS();
    updatePainter();
    
    ////
    // geometry pass
    ////

    m_gBufferFBO->bind();
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_gameMechanics->forEachCuboid([this](const Cuboid * cuboid) {
        // modelMatrix and previous modelMatrix are the same until they will begin to move (e.g. destruction) [motionBlur]
        m_painter.paint(m_cuboidDrawable, cuboid->modelTransform(), cuboid->modelTransform());
    });
    //cave does not move at the moment, so model and prevModel are the same [motionBlur]
    m_cavePainter.paint(m_caveDrawable, glm::mat4(), glm::mat4()); 

    m_gBufferFBO->unbind();
    
    m_previousViewProjection = m_camera.viewProjection();

    ////
    // post processing 
    ////

    //bind gBuffer textures
    m_gBufferNormals->bind(GL_TEXTURE0 + TIU_Normal);
    m_gBufferColor->bind(GL_TEXTURE0 + TIU_Color);
    m_gBufferDepth->bind(GL_TEXTURE0 + TIU_Depth);
    m_gBufferVelocity->bind(GL_TEXTURE0 + TIU_Velocity);

    //SSAO pass
    m_ssaoPostProc.setUniform("projection", m_camera.projection());
    m_ssaoPostProc.setUniform("invProjection", glm::inverse(m_camera.projection()));
    m_ssaoPostProc.setUniform("normalMatrix", m_camera.normal());

    m_ssaoPostProc.apply(*m_ssaoFBO);

    //motion blur pass
    m_motionBlurPostProc.setUniform("currentFPS_targetFPS", fps()/60.f);

    m_ssaoOutput->bind(GL_TEXTURE0 + TIU_SSAO);
    m_motionBlurPostProc.apply(*m_motionBlurFBO);
    m_ssaoOutput->unbind(GL_TEXTURE0 + TIU_SSAO);

    //last pass
    glViewport(0, 0,
        m_camera.viewport().x * devicePixelRatio,
        m_camera.viewport().y * devicePixelRatio);

    m_motionBlurOutput->bind(GL_TEXTURE0 + TIU_MotionBlur);
    m_quadPass.apply(*fbo);
    m_motionBlurOutput->unbind(GL_TEXTURE0 + TIU_MotionBlur);

    //unbind textures
    m_gBufferVelocity->unbind(GL_TEXTURE0 + TIU_Velocity);
    m_gBufferDepth->unbind(GL_TEXTURE0 + TIU_Depth);
    m_gBufferColor->unbind(GL_TEXTURE0 + TIU_Color);
    m_gBufferNormals->unbind(GL_TEXTURE0 + TIU_Normal);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    
    //paint HUD
    fbo->bind();
    m_hud.paint(m_gameMechanics->mammut());
    fbo->unbind();
}

void GameWorldRenderer::initialize()
{
    m_camera.setFovy(80.0);
    m_camera.setZNear(nearPlane);
    m_camera.setZFar(farPlane);

    initializeGBuffer();
    initializePostProcPasses();
}

void GameWorldRenderer::initializeGBuffer()
{
    m_gBufferFBO = new glow::FrameBufferObject();

    m_gBufferColor = Util::create2DTexture();
    m_gBufferNormals = Util::create2DTexture();
    m_gBufferDepth = Util::create2DTexture();
    m_gBufferVelocity = Util::create2DTexture();

    m_gBufferFBO->attachTexture2D(GL_COLOR_ATTACHMENT0, m_gBufferNormals);
    m_gBufferFBO->attachTexture2D(GL_COLOR_ATTACHMENT1, m_gBufferColor);
    m_gBufferFBO->attachTexture2D(GL_COLOR_ATTACHMENT2, m_gBufferVelocity);
    m_gBufferFBO->attachTexture2D(GL_DEPTH_ATTACHMENT, m_gBufferDepth);

    m_gBufferFBO->setDrawBuffers({ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 });
}


void GameWorldRenderer::initializePostProcPasses()
{
    //SSAO
    m_ssaoOutput = Util::create2DTexture();
    m_ssaoPostProc.setInputTextures({ { "color", TIU_Color },
                                      { "normal", TIU_Normal },
                                      { "depth", TIU_Depth }
                                    });

    m_ssaoFBO = new glow::FrameBufferObject();
    m_ssaoFBO->setDrawBuffers( { GL_COLOR_ATTACHMENT0 } );
    m_ssaoFBO->attachTexture2D(GL_COLOR_ATTACHMENT0, m_ssaoOutput);

    //motionBlur
    m_motionBlurOutput = Util::create2DTexture();
    m_motionBlurPostProc.setInputTextures({ { "color", TIU_SSAO },
                                            { "depth", TIU_Depth },
                                            { "velocity", TIU_Velocity }
                                          });

    m_motionBlurFBO = new glow::FrameBufferObject();
    m_motionBlurFBO->setDrawBuffers( { GL_COLOR_ATTACHMENT0 } );
    m_motionBlurFBO->attachTexture2D(GL_COLOR_ATTACHMENT0, m_motionBlurOutput);

    //last pass: use this pass for edge enhancement
    m_quadPass.setFragmentShader("data/shaders/quad.frag");
    m_quadPass.setInputTextures({ { "result", TIU_MotionBlur } });
}

void GameWorldRenderer::resize(int width, int height)
{
    m_camera.setViewport(glm::ivec2(width, height));
    
    m_gBufferNormals->image2D(0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
    m_gBufferColor->image2D(0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
    m_gBufferVelocity->image2D(0, GL_RG16F, width, height, 0, GL_RG, GL_FLOAT, nullptr);
    m_gBufferDepth->image2D(0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    m_ssaoOutput->image2D(0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
    m_motionBlurOutput->image2D(0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);

    m_ssaoPostProc.resize(width, height);
    m_motionBlurPostProc.resize(width, height);
    m_quadPass.resize(width, height);
}

void GameWorldRenderer::updateFPS()
{
    float weightRatio = 0.9f;
    m_avgTimeSinceLastFrame = weightRatio * m_avgTimeSinceLastFrame + (1.0 - weightRatio) * m_lastFrame.elapsed();
    m_lastFrame = QTime::currentTime();
}

void GameWorldRenderer::updatePainter()
{
    m_painter.setViewProjectionUniforms(m_camera.viewProjection(), m_previousViewProjection);
    m_painter.setViewUniform(m_camera.view());
    m_painter.setNearFarUniform(glm::vec2(nearPlane, farPlane));
    m_painter.setEyeUniform(m_camera.eye());

    m_cavePainter.setViewProjectionUniforms(m_camera.viewProjection(), m_previousViewProjection);
    m_cavePainter.setViewUniform(m_camera.view());
    m_cavePainter.setNearFarUniform(glm::vec2(nearPlane, farPlane));
    m_cavePainter.setEyeUniform(m_camera.eye());
}

int GameWorldRenderer::fps() const
{
    return (int) (1000.0 / m_avgTimeSinceLastFrame + 0.5);
}

void GameWorldRenderer::setGameMechanics(const GameMechanics * mechanics)
{
    assert(mechanics != nullptr);
    m_gameMechanics = mechanics;
}

