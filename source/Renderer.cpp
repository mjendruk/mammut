#include "Renderer.h"

#include <glm/gtx/transform.hpp>

#include <QThread>
#include <QVector>

#include "glow/Program.h"
#include "glowutils/File.h"
#include "glow/FrameBufferobject.h"
#include "glowutils/ScreenAlignedQuad.h"

#include "Cuboid.h"
#include "Mammut.h"
#include "Painter.h"
#include "Canvas.h"
#include "GameLogic.h"
#include "GameCamera.h"
#include "RenderCamera.h"
#include "FileAssociatedShader.h"
#include "SSAO.h"

Renderer::Renderer(GameLogic & gameLogic)
:   m_canvas(nullptr)
,   m_gameLogic(gameLogic)
,   m_painter(Painter(m_cuboidDrawable))
,   m_camera(RenderCamera())
,   m_initialized(false)
,   m_DepthProgram(nullptr)
,   m_gBufferFBO(nullptr)
,   m_gBufferDepth(nullptr)
,   m_gBufferNormals(nullptr)
,   m_gBufferColor(nullptr)
,   m_ssaoOutput(nullptr)
,   m_quad(nullptr)
,   m_ssao(nullptr)
{
    QSurfaceFormat format;
    format.setVersion(4, 1);
    format.setDepthBufferSize(24);
    format.setProfile(QSurfaceFormat::CoreProfile);

    m_camera.setFovy(90.0);
    m_camera.setZFar(500.0);
    
    m_canvas = new Canvas(format, this, &m_camera);
    m_canvas->setSwapInterval(Canvas::NoVerticalSyncronization);
    
    m_canvas->setWidth(1024);
    m_canvas->setHeight(768);
    m_canvas->show();
}

Renderer::~Renderer()
{
    delete m_DepthProgram;
    delete m_canvas;
}

void Renderer::render()
{
    m_canvas->beginPaintGL();

    if (!m_initialized)
        initialize();

    // recompile file associated shaders if required
    auto programsWithInvalidatedUniforms(FileAssociatedShader::process());
    m_painter.update(programsWithInvalidatedUniforms);

    m_camera.update(m_gameLogic.camera());
    m_painter.setTransformUniform(m_camera.viewProjection());
    m_painter.setEyeUniform(m_camera.eye());



    m_gBufferFBO->bind();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (Cuboid * cuboid : m_gameLogic.cuboids())
    {
        glm::mat4 scaleMatrix = glm::scale(cuboid->size());
        m_painter.paint(m_cuboidDrawable, cuboid->modelMatrix() * scaleMatrix);
    }

    glm::mat4 scaleMatrix = glm::scale(m_gameLogic.mammut().size());
    m_painter.paint(m_cuboidDrawable, m_gameLogic.mammut().modelMatrix() * scaleMatrix);

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

    /*m_ssao->draw(0, 2, *m_ssaoOutput);


    m_ssaoOutput->bind(GL_TEXTURE3);
    
    m_ssaoOutput->unbind(GL_TEXTURE3);
    */
    m_quad->draw();
    m_gBufferDepth->unbind(GL_TEXTURE2);
    m_gBufferColor->unbind(GL_TEXTURE1);
    m_gBufferNormals->unbind(GL_TEXTURE0);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);


    m_canvas->endPaintGL();
    //We promised to make the rendering slow
    QThread::msleep(10);
}

void Renderer::initialize()
{
    glow::DebugMessageOutput::enable();
    m_painter.initialize();
    m_cuboidDrawable.initialize();
    m_initialized = true;
    m_ssao = new SSAO();

    m_ssaoOutput = new glow::Texture(GL_TEXTURE_2D);
    m_ssaoOutput->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    m_ssaoOutput->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    m_ssaoOutput->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    m_ssaoOutput->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    m_ssaoOutput->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

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

    m_quad = new glow::ScreenAlignedQuad(m_DepthProgram);

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

    resize(m_camera.viewport().x, m_camera.viewport().y);
}

void Renderer::resize(int width, int height)
{
    if (!m_initialized) //we have no context
        return;

    m_gBufferNormals->image2D(0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
    m_gBufferColor->image2D(0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
    m_ssaoOutput->image2D(0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
    m_gBufferDepth->image2D(0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    m_ssao->resize(width, height);
}

void Renderer::registerKeyHandler(QObject & keyHandler)
{
    m_canvas->installEventFilter(&keyHandler);
}
