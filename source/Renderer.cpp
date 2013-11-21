#include "Renderer.h"

#include <glm/gtx/transform.hpp>

#include <QThread>
#include <QVector>

#include "Cuboid.h"
#include "Painter.h"
#include "Canvas.h"
#include "GameLogic.h"
#include "RenderCamera.h"

Renderer::Renderer(GameLogic & gameLogic)
:   m_canvas(nullptr)
,   m_gameLogic(gameLogic)
,   m_painter(Painter(m_cuboidDrawable))
, m_initialized(false)
{
    QSurfaceFormat format;
    format.setVersion(4, 1);
    format.setDepthBufferSize(24);
    format.setProfile(QSurfaceFormat::CoreProfile);

    m_canvas = new Canvas(format);
    m_canvas->setSwapInterval(Canvas::NoVerticalSyncronization);
    
    m_canvas->setWidth(1024);
    m_canvas->setHeight(768);
    
    m_canvas->assignPainter(&m_painter);
    m_canvas->show();

    m_cuboids = &gameLogic.cuboids();
    
    //quick hack for navigation
    gameLogic.assignCamera(m_canvas->m_camera.data());
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

    for (Cuboid * cuboid : *m_cuboids)
    {
        glm::mat4 scaleMatrix = glm::scale(cuboid->size());
        m_painter.paint(m_cuboidDrawable, cuboid->modelMatrix() * scaleMatrix);
    }
    m_canvas->endPaintGL();

    //do stuff with m_gameLogic

    //We promised to make the rendering slow
    QThread::msleep(10);
}

void Renderer::initialize()
{
    m_cuboidDrawable.initialize();
    m_initialized = true;
}

void Renderer::registerKeyHandler(QObject & keyHandler)
{
    m_canvas->installEventFilter(&keyHandler);
}
