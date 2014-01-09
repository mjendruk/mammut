#include "Renderer.h"

#include <glm/gtx/transform.hpp>

#include "glow/Program.h"
#include "glowutils/File.h"
#include "glow/FrameBufferobject.h"
#include "glowutils/ScreenAlignedQuad.h"

#include "../gamelogic/Cuboid.h"
#include "../gamelogic/Mammut.h"
#include "../gamelogic/GameLogic.h"
#include "../gamelogic/GameCamera.h"
#include "Painter.h"
#include "RenderCamera.h"
#include "FileAssociatedShader.h"
#include "SSAO.h"
#include "CharacterDrawable.h"
#include "Canvas.h"

const float Renderer::nearPlane = 0.1f;
const float Renderer::farPlane = 700.0f;

Renderer::Renderer(GameLogic & gameLogic, Canvas & canvas)
:   m_gameLogic(gameLogic)
,   m_canvas(canvas)
,   m_DepthProgram(nullptr)
,   m_gBufferFBO(nullptr)
,   m_gBufferDepth(nullptr)
,   m_gBufferNormals(nullptr)
,   m_gBufferColor(nullptr)
,   m_ssaoOutput(nullptr)
,   m_quad(nullptr)
,   m_ssao(nullptr)
{
}

Renderer::~Renderer()
{
}

void Renderer::paint()
{
    glViewport(0, 0, m_camera.viewport().x, m_camera.viewport().y);
    
    // recompile file associated shaders if required
    auto programsWithInvalidatedUniforms(FileAssociatedShader::process());
    m_painter.update(programsWithInvalidatedUniforms);
    
    m_camera.update(m_gameLogic.camera());
    m_painter.setViewProjectionUniform(m_camera.viewProjection());
    m_painter.setViewUniform(m_camera.view());
    m_painter.setNearFarUniform(glm::vec2(nearPlane, farPlane));
    m_painter.setEyeUniform(m_camera.eye());
    
    m_caveDrawable.update(m_camera.eye());
    
    m_gBufferFBO->bind();
    
    glViewport(0, 0, m_camera.viewport().x, m_camera.viewport().y);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    m_gameLogic.forEachCuboid([this](Cuboid & cuboid) {
        m_painter.paint(m_cuboidDrawable, cuboid.modelTransform());
    });
    
    m_painter.paint(m_cuboidDrawable, m_gameLogic.mammut().modelTransform());
    
    m_painter.paint(m_caveDrawable, glm::mat4());
    
    m_gBufferFBO->unbind();
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //post
    
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    
    m_DepthProgram->setUniform("normal", 0);
    m_DepthProgram->setUniform("color", 1);
    m_DepthProgram->setUniform("depth", 2);
    m_DepthProgram->setUniform("ssaoOutput", 3);
    m_DepthProgram->setUniform("transformi", m_camera.viewProjectionInverted());
    
    m_gBufferNormals->bind(GL_TEXTURE0);
    m_gBufferColor->bind(GL_TEXTURE1);
    m_gBufferDepth->bind(GL_TEXTURE2);
    
    m_ssao->draw(0, 2, m_camera.normal(), m_camera.projection(), *m_ssaoOutput);
    
    glViewport(0, 0,
               m_camera.viewport().x * m_canvas.devicePixelRatio(),
               m_camera.viewport().y * m_canvas.devicePixelRatio());
    m_ssaoOutput->bind(GL_TEXTURE3);
    m_quad->draw();
    m_ssaoOutput->unbind(GL_TEXTURE3);
    
    m_gBufferDepth->unbind(GL_TEXTURE2);
    m_gBufferColor->unbind(GL_TEXTURE1);
    m_gBufferNormals->unbind(GL_TEXTURE0);
    
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    
    glDisable(GL_DEPTH_TEST);
    float aspectRatio = m_camera.aspectRatio();
    QString velocityString = QString("Velocity: %1").arg(m_gameLogic.mammut().velocity(), 0, 'g', 2);
    m_stringDrawer.paint(velocityString, glm::translate(-0.95f, -0.9f, 0.0f) *
                                         glm::scale(1 / aspectRatio, 1.0f, 0.0f));
    glEnable(GL_DEPTH_TEST);
}

void Renderer::initialize()
{
    glow::DebugMessageOutput::enable();
    m_painter.initialize();
    m_cuboidDrawable.initialize();
    m_caveDrawable.initialize();
    
    m_ssao = new SSAO();

    m_ssaoOutput = new glow::Texture(GL_TEXTURE_2D);
    m_ssaoOutput->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    m_ssaoOutput->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    m_ssaoOutput->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    m_ssaoOutput->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    m_ssaoOutput->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    m_stringDrawer.initialize();
    
    m_camera.setFovy(90.0);
    m_camera.setZNear(nearPlane);
    m_camera.setZFar(farPlane);
    
    initializeGBuffer();
}

void Renderer::initializeGBuffer()
{
    m_DepthProgram = new glow::Program();

    glow::Shader * frag = FileAssociatedShader::getOrCreate(
        GL_FRAGMENT_SHADER, "data/quad.frag", *m_DepthProgram);
    glow::Shader * vert = FileAssociatedShader::getOrCreate(
        GL_VERTEX_SHADER, "data/quad.vert", *m_DepthProgram);
    m_DepthProgram->link();

    m_quad = new glowutils::ScreenAlignedQuad(m_DepthProgram);

    m_gBufferFBO = new glow::FrameBufferObject();

    m_gBufferColor = new glow::Texture(GL_TEXTURE_2D);
    m_gBufferColor->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    m_gBufferColor->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    m_gBufferColor->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    m_gBufferColor->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    m_gBufferColor->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    m_gBufferNormals = new glow::Texture(GL_TEXTURE_2D);
    m_gBufferNormals->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    m_gBufferNormals->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    m_gBufferNormals->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    m_gBufferNormals->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    m_gBufferNormals->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    m_gBufferDepth = new glow::Texture(GL_TEXTURE_2D);
    m_gBufferDepth->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    m_gBufferDepth->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    m_gBufferDepth->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    m_gBufferDepth->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    m_gBufferDepth->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    m_gBufferFBO->attachTexture2D(GL_COLOR_ATTACHMENT0, m_gBufferNormals);
    m_gBufferFBO->attachTexture2D(GL_COLOR_ATTACHMENT1, m_gBufferColor);
    m_gBufferFBO->attachTexture2D(GL_DEPTH_ATTACHMENT, m_gBufferDepth);

    m_gBufferFBO->setDrawBuffers({ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 });
}

void Renderer::resize(int width, int height)
{
    m_camera.setViewport(glm::ivec2(width, height));
    
    m_gBufferNormals->image2D(0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
    m_gBufferColor->image2D(0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
    m_ssaoOutput->image2D(0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
    m_gBufferDepth->image2D(0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    m_ssao->resize(width, height);
}