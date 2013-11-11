
#include "Application.h"

#include <iostream>
#include <QTimer>

#include "Painter.h"
#include "Canvas.h"

Application::Application(
    int & argc
,   char ** argv)
:   AbstractApplication(argc, argv)
,   m_canvas(nullptr)
,   m_painter(new Painter())
,   m_loop(false)
{
    QSurfaceFormat format;
    format.setVersion(4, 1);
    format.setProfile(QSurfaceFormat::CoreProfile);

    m_canvas = new Canvas(format);
    m_canvas->setSwapInterval(Canvas::NoVerticalSyncronization);

    m_canvas->assignPainter(m_painter);
    m_canvas->show();

    QTimer::singleShot(0, this, SLOT(run()));
}

Application::~Application()
{
    delete m_canvas;
    delete m_painter;
}

void Application::run()
{
    m_loop = true;

    while(m_loop) {
        processEvents();
        m_canvas->paintGL();
    }
}
