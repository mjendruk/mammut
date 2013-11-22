#include "Renderer.h"

#include <glm/gtx/transform.hpp>

#include <QThread>
#include <QVector>

#include "Cuboid.h"
#include "Mammoth.h"
#include "Painter.h"
#include "Canvas.h"
#include "GameLogic.h"
#include "RenderCamera.h"
#include "FileAssociatedShader.h"

Renderer::Renderer(GameLogic & gameLogic)
:   m_canvas(nullptr)
,   m_gameLogic(gameLogic)
,   m_painter(Painter(m_cuboidDrawable))
,   m_camera(RenderCamera())
,   m_initialized(false)
{
    QSurfaceFormat format;
    format.setVersion(4, 1);
    format.setDepthBufferSize(24);
    format.setProfile(QSurfaceFormat::CoreProfile);

    m_camera.setFovy(90.0);
    m_camera.setZFar(500.0);
    m_canvas = new Canvas(format, nullptr, &m_camera);

    m_canvas->setSwapInterval(Canvas::NoVerticalSyncronization);
    
    m_canvas->setWidth(1024);
    m_canvas->setHeight(768);
    m_canvas->show();
}

Renderer::~Renderer()
{
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

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (Cuboid * cuboid : m_gameLogic.cuboids())
    {
        glm::mat4 scaleMatrix = glm::scale(cuboid->size());
        m_painter.paint(m_cuboidDrawable, cuboid->modelMatrix() * scaleMatrix);
    }

    glm::mat4 scaleMatrix = glm::scale(m_gameLogic.mammoth().size());
    m_painter.paint(m_cuboidDrawable, m_gameLogic.mammoth().modelMatrix() * scaleMatrix);

    m_canvas->endPaintGL();

    //do stuff with m_gameLogic

    //We promised to make the rendering slow
    QThread::msleep(10);
}

void Renderer::initialize()
{
    m_painter.initialize();
    m_cuboidDrawable.initialize();
    m_initialized = true;
}

void Renderer::registerKeyHandler(QObject & keyHandler)
{
    m_canvas->installEventFilter(&keyHandler);
}
