#include "Renderer.h"

#include <QThread>

#include "Painter.h"
#include "Canvas.h"
#include "GameLogic.h"

Renderer::Renderer(GameLogic & gameLogic)
:   m_canvas(nullptr)
,   m_painter(Painter())
,   m_gameLogic(gameLogic)
{

    QSurfaceFormat format;
    format.setVersion(4, 1);
    format.setDepthBufferSize(24);
    format.setProfile(QSurfaceFormat::CoreProfile);

    m_canvas = new Canvas(format);
    m_canvas->setSwapInterval(Canvas::NoVerticalSyncronization);
    
    m_canvas->setWidth(1024);
    m_canvas->setHeight(768);
    
    m_painter.m_cuboids = &m_gameLogic.cuboids();
    m_canvas->assignPainter(&m_painter);
    m_canvas->show();
}

Renderer::~Renderer()
{
    delete m_canvas;
}

void Renderer::render()
{
    m_canvas->paintGL();

    //do stuff with m_gameLogic

    //We promised to make the rendering slow
    QThread::msleep(10);
}


void Renderer::registerKeyHandler(KeyHandler & keyHandler)
{
    m_canvas->installEventFilter(&keyHandler);
}