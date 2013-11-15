#include "Renderer.h"

#include <QThread>
#include "Painter.h"
#include "Canvas.h"

Renderer::Renderer(GameLogic * gameLogic)
:   m_canvas(nullptr)
,   m_painter(new Painter())
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
    
    m_canvas->assignPainter(m_painter);
    m_canvas->show();
}

Renderer::~Renderer()
{
    delete m_canvas;
    delete m_painter;
}

void Renderer::render()
{
	m_canvas->paintGL();

	//do stuff with m_gameLogic

	//We promised to make the rendering slow
	QThread::msleep(10);
}