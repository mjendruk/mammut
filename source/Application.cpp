
#include <GL/glew.h>

#include "Viewer.h"
#include "Canvas.h"

#include "Application.h"
#include "Painter.h"


Application::Application(
    int & argc
,   char ** argv)
:   AbstractApplication(argc, argv)
,   m_viewer(new Viewer())
,   m_canvas(nullptr)
,   m_painter(new Painter())
{
    Format format;

	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setVersion(4, 3);
    
    m_canvas = new Canvas(format);
	m_viewer->setCentralWidget(m_canvas);

	m_canvas->attachPainter(m_painter);
	m_canvas->setActivePainter(m_painter);
	m_canvas->setContinuousRepaint(true);

    m_viewer->setWindowTitle(title());
    m_viewer->show();
}

Application::~Application()
{
	delete m_painter;
	delete m_canvas;
    delete m_viewer;
}