
#include <cassert>

#include <QApplication>
#include <QBasicTimer>
#include <QResizeEvent>

#include "Canvas.h"
#include "AbstractPainter.h"
#include "AbstractContext.h"

#include "PrintPainter.h"


namespace
{
    const int CONTINUOUS_REPAINT_DELAY = 1000 / 60;
}

Canvas::Canvas(
	const Format & format
,	QWidget * parent)
:   QGLWidget(Canvas::fromFormat(format), parent)
,   m_timer(nullptr)
,   m_initialized(false)
,   m_continuousRepaint(false)
,	m_activePainter(nullptr)
,	m_requested(format)
{
    m_timer = new QBasicTimer();

    setAutoFillBackground(false); // Important for overdraw, not occluding the scene.
    setMinimumSize(1, 1); 

	// Todo: remove in workin future architecture
	//setAutoBufferSwap(true);
}

Canvas::~Canvas()
{
	delete m_timer;
}

//void Canvas::setFormat(const Format & format)
//{
//	if (m_initialized)
//	{
//		qWarning("Surface format cannot be changed after canvas initialization.");
//		return;
//	}
//	m_format = format;
//}

const Format & Canvas::format() const
{
	return m_requested;
    //return m_format;
}

void Canvas::setContinuousRepaint(const bool enable)
{
	if (enable == m_continuousRepaint)
		return;
	
	m_continuousRepaint = enable;
	if (m_continuousRepaint)
		updateGL();
}

bool Canvas::continuousRepaint() const
{
	return m_continuousRepaint;
}

void Canvas::initializeGL()
{
	if (m_initialized)
		return;

	Painters::const_iterator i = m_painters.cbegin();
	const Painters::const_iterator iEnd = m_painters.cend();

	for (; i != iEnd; ++i)
		(*i)->initialize(format());

	// TODO: the verification should be done by the painter in future architecture
	const Format created = toFormat(QGLWidget::format());
	AbstractContext::verify(format(), created);

	m_initialized = true;

    if(m_continuousRepaint)
        m_timer->start(CONTINUOUS_REPAINT_DELAY, this);
}

const bool Canvas::attachPainter(AbstractPainter * painter)
{
    if(!painter || m_painters.contains(painter))
        return false;

    if(m_initialized)
		painter->initialize(format());

	m_painters.append(painter);
    return true;
}

const bool Canvas::dettachPainter(AbstractPainter * painter)
{
    if(!painter || !m_painters.contains(painter))
        return false;

    m_painters.removeOne(painter);
    return true;
}

const bool Canvas::setActivePainter(AbstractPainter * painter)
{
    if(!painter || !m_painters.contains(painter))
        return false;

    m_activePainter = painter;
    return true;
}

void Canvas::showEvent(QShowEvent * event)
{
	//assert(m_initialized);
    //if(!m_initialized)
    //    initialize();

    Painters::const_iterator i = m_painters.cbegin();
    const Painters::const_iterator iEnd = m_painters.cend();

    for(; i != iEnd; ++i)
        (*i)->show();

}

void Canvas::hideEvent(QHideEvent *)
{
    Painters::const_iterator i = m_painters.cbegin();
    const Painters::const_iterator iEnd = m_painters.cend();

    for(; i != iEnd; ++i)
        (*i)->hide();

}

void Canvas::resizeEvent(QResizeEvent * event)
{
    Painters::const_iterator i = m_painters.cbegin();
    const Painters::const_iterator iEnd = m_painters.cend();

    for(; i != iEnd; ++i)
        (*i)->resize(event->size().width(), event->size().height());
    
}

void Canvas::paintGL()
{
	if (m_activePainter)
	{
		//	assert(m_activePainter->context());

		//m_activePainter->context()->makeCurrent();
	}
}

//void Canvas::paintEvent(QPaintEvent *)
//{
//	//if (m_activePainter)
//	//{
//	//	assert(m_activePainter->context());
//
//	//	m_activePainter->context()->makeCurrent();
//	//	m_activePainter->paint(m_debug ? 0 : 0); // TODO: setup target ...
//	//	m_activePainter->context()->doneCurrent();
//	//}
//    
//}

void Canvas::timerEvent(QTimerEvent * event)
{
    assert(m_timer);
    if(event->timerId() != m_timer->timerId())
        return;

    updateGL();
}

const QGLFormat Canvas::fromFormat(const Format & format)
{
	QGLFormat qFormat;

	qFormat.setDepthBufferSize(format.depthBufferSize());

	qFormat.setRedBufferSize(format.redBufferSize());
	qFormat.setGreenBufferSize(format.greenBufferSize());
	qFormat.setBlueBufferSize(format.blueBufferSize());

	qFormat.setAlphaBufferSize(format.alphaBufferSize());

	qFormat.setSamples(format.samples());

	qFormat.setVersion(format.majorVersion(), format.minorVersion());

	switch (format.profile())
	{
	case QSurfaceFormat::NoProfile:
		qFormat.setProfile(QGLFormat::NoProfile);
		break;
	case QSurfaceFormat::CoreProfile:
		qFormat.setProfile(QGLFormat::CoreProfile);
		break;
	case QSurfaceFormat::CompatibilityProfile:
		qFormat.setProfile(QGLFormat::CompatibilityProfile);
		break;
	}

	qFormat.setStencilBufferSize(format.stencilBufferSize());
	qFormat.setStereo(format.stereo());

	switch (format.swapBehavior())
	{
	case QSurfaceFormat::SingleBuffer:
		qFormat.setDoubleBuffer(false);
		break;
	default:
		qDebug("Selected swap behavior is not yet supported. Double buffering is used instead.");
	case QSurfaceFormat::DoubleBuffer:
		qFormat.setDoubleBuffer(true);
		break;
	}

	return qFormat;
}

const Format Canvas::toFormat(const QGLFormat & qFormat)
{
	Format format;

	format.setDepthBufferSize(qFormat.depthBufferSize());

	format.setRedBufferSize(qFormat.redBufferSize());
	format.setGreenBufferSize(qFormat.greenBufferSize());
	format.setBlueBufferSize(qFormat.blueBufferSize());

	format.setAlphaBufferSize(qFormat.alphaBufferSize());

	format.setSamples(qFormat.samples());

	format.setVersion(qFormat.majorVersion(), qFormat.minorVersion());

	switch (qFormat.profile())
	{
	case QGLFormat::NoProfile:
		format.setProfile(QSurfaceFormat::NoProfile);
		break;
	case QGLFormat::CoreProfile:
		format.setProfile(QSurfaceFormat::CoreProfile);
		break;
	case QGLFormat::CompatibilityProfile:
		format.setProfile(QSurfaceFormat::CompatibilityProfile);
		break;
	}

	format.setStencilBufferSize(qFormat.stencilBufferSize());
	format.setStereo(qFormat.stereo());

	format.setSwapBehavior(qFormat.doubleBuffer() ?
		QSurfaceFormat::DoubleBuffer : QSurfaceFormat::SingleBuffer);

	return format;
}
