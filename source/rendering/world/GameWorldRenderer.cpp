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
#include "SSAOPass.h"
#include "PostprocessingPass.h"

const float GameWorldRenderer::nearPlane = 0.1f;
const float GameWorldRenderer::farPlane = 700.0f;

GameWorldRenderer::GameWorldRenderer()
:   m_hud(m_camera, *this)
,   m_lastFrame(QTime::currentTime())
,   m_gameMechanics(nullptr)
{
    initialize();
}

GameWorldRenderer::~GameWorldRenderer()
{
}

void GameWorldRenderer::render(glow::FrameBufferObject * fbo, float devicePixelRatio)
{
    assert(m_gameMechanics != nullptr);
    
    updateFPS();

    glViewport(0, 0, m_camera.viewport().x, m_camera.viewport().y);
    
    // recompile file associated shaders if required
    auto programsWithInvalidatedUniforms(FileAssociatedShader::process());
    m_painter.update(programsWithInvalidatedUniforms);
    m_cavePainter.update(programsWithInvalidatedUniforms);
    
    m_camera.update(m_gameMechanics->camera());
    m_painter.setViewProjectionUniform(m_camera.viewProjection());
    m_painter.setViewUniform(m_camera.view());
    m_painter.setNearFarUniform(glm::vec2(nearPlane, farPlane));
    m_painter.setEyeUniform(m_camera.eye());
    
    m_cavePainter.setViewProjectionUniform(m_camera.viewProjection());
    m_cavePainter.setViewUniform(m_camera.view());
    m_cavePainter.setNearFarUniform(glm::vec2(nearPlane, farPlane));
    m_cavePainter.setEyeUniform(m_camera.eye());

    m_caveDrawable.update(m_camera.eye());
    
    // geometry pass
    m_gBufferFBO->bind();
    
    glViewport(0, 0, m_camera.viewport().x, m_camera.viewport().y);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    m_gameMechanics->forEachCuboid([this](const Cuboid * cuboid) {
        m_painter.paint(m_cuboidDrawable, cuboid->modelTransform());
    });
    m_painter.paint(m_cuboidDrawable, m_gameMechanics->mammut().modelTransform());
    m_cavePainter.paint(m_caveDrawable, glm::mat4());
    
    m_gBufferFBO->unbind();
    
    //post processing 

    m_gBufferNormals->bind(GL_TEXTURE0 + 0);
    m_gBufferColor->bind(GL_TEXTURE0 + 1);
    m_gBufferDepth->bind(GL_TEXTURE0 + 2);

    //SSAO pass
    m_ssaoPass->setUniform("projection", m_camera.projection());
    m_ssaoPass->setUniform("invProjection", glm::inverse(m_camera.projection()));
    m_ssaoPass->setUniform("normalMatrix", m_camera.normal());

    m_ssaoPass->apply(*m_ssaoFBO);

    glViewport(0, 0,
               m_camera.viewport().x * devicePixelRatio,
               m_camera.viewport().y * devicePixelRatio);

    //last pass
    m_ssaoOutput->bind(GL_TEXTURE0 + 3);
    m_quadPass->setUniform("transformi", m_camera.viewProjectionInverted());
    m_quadPass->apply(*fbo);
    m_ssaoOutput->unbind(GL_TEXTURE0 + 3);

    m_gBufferDepth->unbind(GL_TEXTURE0 + 2);
    m_gBufferColor->unbind(GL_TEXTURE0 + 1);
    m_gBufferNormals->unbind(GL_TEXTURE0 + 0);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    
    //paint HUD
    fbo->bind();
    m_hud.paint(m_gameMechanics->mammut());
    fbo->unbind();
}

void GameWorldRenderer::initialize()
{
    m_painter.initialize();
    m_cavePainter.initialize();
    m_cuboidDrawable.initialize();
    m_caveDrawable.initialize();
    m_hud.initialize();

    m_camera.setFovy(90.0);
    m_camera.setZNear(nearPlane);
    m_camera.setZFar(farPlane);

    initializeGBuffer();

    //initialize postprocessing passes
    m_quadPass = new PostprocessingPass("quad");
    m_quadPass->setVertexShader("data/quad.vert");
    m_quadPass->setFragmentShader("data/quad.frag");
    m_quadPass->setInputTextures({ { "color", TIU_Color }, 
                                   { "ssaoOutput", TIU_SSAO } 
                                 });
    m_quadPass->set2DTextureOutput({}); //render on screen

    m_ssaoOutput = create2DTexture();
    m_ssaoPass = new SSAOPass("ssaoPass");
    m_ssaoPass->setInputTextures({ { "normal", TIU_Normal },
                                   { "depth", TIU_Depth }
                                 });
    m_ssaoPass->set2DTextureOutput({ { GL_COLOR_ATTACHMENT0, m_ssaoOutput } });

    m_ssaoFBO = new glow::FrameBufferObject();
}

void GameWorldRenderer::initializeGBuffer()
{
    m_gBufferFBO = new glow::FrameBufferObject();

    m_gBufferColor = create2DTexture();
    m_gBufferNormals = create2DTexture();
    m_gBufferDepth = create2DTexture();

    m_gBufferFBO->attachTexture2D(GL_COLOR_ATTACHMENT0, m_gBufferNormals);
    m_gBufferFBO->attachTexture2D(GL_COLOR_ATTACHMENT1, m_gBufferColor);
    m_gBufferFBO->attachTexture2D(GL_DEPTH_ATTACHMENT, m_gBufferDepth);

    m_gBufferFBO->setDrawBuffers({ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 });
}

void GameWorldRenderer::resize(int width, int height)
{
    m_camera.setViewport(glm::ivec2(width, height));
    
    m_gBufferNormals->image2D(0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
    m_gBufferColor->image2D(0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
    m_ssaoOutput->image2D(0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
    m_gBufferDepth->image2D(0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    m_ssaoPass->resize(width, height);
    m_quadPass->resize(width, height);
}

void GameWorldRenderer::updateFPS()
{
    float weightRatio = 0.9f;
    m_avgTimeSinceLastFrame = weightRatio * m_avgTimeSinceLastFrame + (1.0 - weightRatio) * m_lastFrame.elapsed();
    m_lastFrame = QTime::currentTime();
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

glow::ref_ptr<glow::Texture> GameWorldRenderer::create2DTexture()
{
    glow::ref_ptr<glow::Texture> texture =  new glow::Texture(GL_TEXTURE_2D);
    texture->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    texture->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    texture->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    texture->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    texture->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    return texture;
}
