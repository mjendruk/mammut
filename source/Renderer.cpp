#include "Renderer.h"

#include <glm/gtx/transform.hpp>

#include <QThread>
#include <QVector>

#include "Cuboid.h"
#include "Mammut.h"
#include "Painter.h"
#include "Canvas.h"
#include "GameLogic.h"
#include "GameCamera.h"
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
    
    m_canvas = new Canvas(format, &m_camera);
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
    m_painter.setEyeUniform(m_camera.eye());

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_gameLogic.forEachCuboid([this](Cuboid & cuboid) {
        m_painter.paint(m_cuboidDrawable, cuboid.modelTransform());
    });

    m_painter.paint(m_cuboidDrawable, m_gameLogic.mammut().modelTransform());

    m_canvas->endPaintGL();
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
