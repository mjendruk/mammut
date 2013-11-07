
#include <cassert>

#include <QDebug>
#include <QApplication>
#include <QBasicTimer>
#include <QResizeEvent>

#include "AbstractPainter.h"
#include "AdaptiveGrid.h"
#include "FileAssociatedShader.h"
#include "Camera.h"
#include "Navigation.h"
#include "NavigationMath.h"
#include "Timer.h"
#include "CyclicTime.h"
#include "Canvas.h"


Canvas::Canvas(
    const QSurfaceFormat & format
,   QScreen * screen)
: QWindow(screen)
, m_context(new QOpenGLContext)
, m_painter(nullptr)
, m_camera(new Camera())
, m_navigation(new Navigation(*m_camera))
, m_swapInterval(VerticalSyncronization)
, m_repaintTimer(new QBasicTimer())
, m_continuousRepaint(false)
, m_fpsTimer(nullptr)
, m_time(new CyclicTime(0.0L, 60.0)) // this is one day in 60 seconds (1d/1h)
, m_swapts(0.0)
, m_swaps(0)
, m_update(false)
{
    setSurfaceType(OpenGLSurface); 

    create();

    m_camera->setFovy(40.0);
    m_navigation->reset();

    initializeGL(format);
}

Canvas::~Canvas()
{
}

QSurfaceFormat Canvas::format() const
{
    if (!m_context)
        return QSurfaceFormat();

    return m_context->format();
}

void Canvas::setContinuousRepaint(
    bool enable
,   int msec)
{
    if (m_continuousRepaint)
        m_repaintTimer->stop();

	m_continuousRepaint = enable;

    if (m_continuousRepaint)
        m_repaintTimer->start(msec, this);
}

bool Canvas::continuousRepaint() const
{
	return m_continuousRepaint;
}

const QString Canvas::querys(const GLenum penum) 
{
    const QString result = reinterpret_cast<const char*>(glGetString(penum));
    //glError();

    return result;
}

const GLint Canvas::queryi(const GLenum penum)
{
    GLint result;
    glGetIntegerv(penum, &result);

    return result;
}

void Canvas::initializeGL(const QSurfaceFormat & format)
{
    m_context->setFormat(format);
    if (!m_context->create())
	{
		qCritical() << "Errors during creation of OpenGL context.";
		return;
	}

    m_context->makeCurrent(this);
    if (!initializeOpenGLFunctions())
    {
        qCritical() << "Initializing OpenGL failed.";
        return;
    }

    // print some hardware information

    qDebug();
    qDebug().nospace() << "GPU: " 
        << qPrintable(querys(GL_RENDERER)) << " ("
        << qPrintable(querys(GL_VENDOR)) << ", "
        << qPrintable(querys(GL_VERSION)) << ")";
    qDebug().nospace() << "GL Version: "
        << qPrintable(QString::number(queryi(GL_MAJOR_VERSION))) << "."
        << qPrintable(QString::number(queryi(GL_MINOR_VERSION))) << " "
        << (queryi(GL_CONTEXT_CORE_PROFILE_BIT) ? "Core" : "Compatibility");
    qDebug();

    verifyExtensions(); // false if no painter ...

    m_grid.reset(new AdaptiveGrid(*this));
    m_grid->setNearFar(m_camera->zNear(), m_camera->zFar());

    connect(m_camera.data(), &Camera::changed, this, &Canvas::cameraChanged);

    m_context->doneCurrent();

    m_time->setf(0.0);
    m_time->start();
}

void Canvas::resizeEvent(QResizeEvent * event)
{
    if (!m_painter)
        return;

    m_camera->setViewport(event->size());

    m_context->makeCurrent(this);

    m_painter->resize(event->size().width(), event->size().height());
    m_grid->update(m_camera->eye(), m_camera->viewProjection());

    m_context->doneCurrent();

    if (isExposed() && Hidden != visibility())
        paintGL();
}

void Canvas::paintGL()
{
    if (!m_painter || !isExposed() || Hidden == visibility())
        return;

    m_context->makeCurrent(this);
    auto programsWithInvalidatedUniforms(FileAssociatedShader::process()); // recompile file associated shaders if required

    if (m_update)
    {
        m_painter->update();
        m_grid->update(m_camera->eye(), m_camera->viewProjection());

        m_update = false;
    }
    else
        m_painter->update(programsWithInvalidatedUniforms);

	m_painter->paint(m_time->getf(true));
    m_grid->draw(*this);

    m_context->swapBuffers(this);
    m_context->doneCurrent();

    emit timeUpdate(m_time->getf());

    if (!m_fpsTimer)
    {
        m_fpsTimer.reset(new Timer(true, false));
        m_swapts = 0.0;
    }
    else
        m_fpsTimer->update();

    ++m_swaps;

    if (m_fpsTimer->elapsed() - m_swapts >= 1e+9)
    {
        const float fps = 1e+9f * static_cast<float>(static_cast<long double>
            (m_swaps) / (m_fpsTimer->elapsed() - m_swapts));

        emit fpsUpdate(fps);

        m_swapts = m_fpsTimer->elapsed();
        m_swaps = 0;
    }
}

void Canvas::cameraChanged()
{
    m_update = true;
}

void Canvas::timerEvent(QTimerEvent * event)
{
    assert(m_repaintTimer);

    if(event->timerId() != m_repaintTimer->timerId())
        return;

    paintGL();
}

void Canvas::assignPainter(AbstractPainter * painter)
{
    if (m_painter == painter)
        return;

    m_painter = painter;
    if (!m_painter)
        return;

    m_context->makeCurrent(this);

    m_painter->initialize();
    m_painter->setCamera(m_camera.data());

    verifyExtensions();

    m_context->doneCurrent();

    m_navigation->setCoordinateProvider(m_painter);
}

bool Canvas::verifyExtensions() const
{
    if (!m_painter)
        return false;

    if (!m_context->isValid())
    {
        qWarning("Extensions cannot be verified due to invalid context.");
        return false;
    }

    QStringList unsupported;

    const QStringList & extensions(m_painter->extensions());
    foreach(const QString & extension, extensions)
    if (!m_context->hasExtension(qPrintable(extension)))
        unsupported << extension;

    if (unsupported.isEmpty())
        return true;

    if (unsupported.size() > 1)
        qWarning("The following mandatory OpenGL extensions are not supported:");
    else
        qWarning("The following mandatory OpenGL extension is not supported:");

    foreach(const QString & extension, unsupported)
        qWarning() << extension;

    qWarning("");

    return false;
}

void Canvas::setSwapInterval(SwapInterval swapInterval)
{
    m_context->makeCurrent(this);

    bool result(false);
    m_swapInterval = swapInterval;

#ifdef WIN32

    // ToDo: C++11 - type aliases
    typedef bool(WINAPI * SWAPINTERVALEXTPROC) (int);
    static SWAPINTERVALEXTPROC wglSwapIntervalEXT = nullptr;

    if (!wglSwapIntervalEXT)
        wglSwapIntervalEXT = reinterpret_cast<SWAPINTERVALEXTPROC>(m_context->getProcAddress("wglSwapIntervalEXT"));
    if (wglSwapIntervalEXT)
        result = wglSwapIntervalEXT(swapInterval);

#elif __APPLE__

    qWarning("ToDo: Setting swap interval is currently not implemented for __APPLE__");

#else
    // ToDo: C++11 - type aliases
    typedef int(APIENTRY * SWAPINTERVALEXTPROC) (int);
    static SWAPINTERVALEXTPROC glXSwapIntervalSGI = nullptr;

    if (!glXSwapIntervalSGI)
        glXSwapIntervalSGI = reinterpret_cast<SWAPINTERVALEXTPROC>(m_context->getProcAddress("glXSwapIntervalSGI"));
    if (glXSwapIntervalSGI)
        result = glXSwapIntervalSGI(swapInterval);

#endif

    if (!result)
        qWarning("Setting swap interval to %s failed."
            , qPrintable(swapIntervalToString(swapInterval)));
    else
        qDebug("Setting swap interval to %s."
            , qPrintable(swapIntervalToString(swapInterval)));

    m_context->doneCurrent();
}

void Canvas::toggleSwapInterval()
{
    switch (m_swapInterval)
    {
    case NoVerticalSyncronization:
        setSwapInterval(VerticalSyncronization);
        break;
    case VerticalSyncronization:
        setSwapInterval(AdaptiveVerticalSyncronization);
        break;
    case AdaptiveVerticalSyncronization:
        setSwapInterval(NoVerticalSyncronization);
        break;
    }
}

const QString Canvas::swapIntervalToString(SwapInterval swapInterval)
{
    switch (swapInterval)
    {
    case NoVerticalSyncronization:
        return QString("NoVerticalSyncronization");
    case VerticalSyncronization:
        return QString("VerticalSyncronization");
    case AdaptiveVerticalSyncronization:
        return QString("AdaptiveVerticalSyncronization");
    default:
        return QString();
    }
}


void Canvas::keyPressEvent(QKeyEvent * event)
{
    if (!m_navigation)
        return;

    m_navigation->keyPressEvent(event);

	// forward event to painter for exercise mode switching
	m_painter->keyPressEvent(event);
}
void Canvas::keyReleaseEvent(QKeyEvent * event)
{
    if (!m_navigation)
        return;

    m_navigation->keyReleaseEvent(event);
}

void Canvas::mouseMoveEvent(QMouseEvent * event)
{
    if (!m_navigation)
        return;

    m_context->makeCurrent(this);
    m_navigation->mouseMoveEvent(event);

    emit mouseUpdate(event->pos());
    if (m_painter)
    {
        if (NavigationMath::validDepth(m_painter->depthAt(event->pos())))
            emit objUpdate(m_painter->objAt(event->pos()));
        else
            emit objUpdate(QVector3D());
    }
    m_context->doneCurrent();        
}

void Canvas::mousePressEvent(QMouseEvent * event)
{
    if (!m_navigation)
        return;

    m_context->makeCurrent(this);
    m_navigation->mousePressEvent(event);
    m_context->doneCurrent();
}

void Canvas::mouseReleaseEvent(QMouseEvent * event)
{
    if (!m_navigation)
        return;

    m_context->makeCurrent(this);
    m_navigation->mouseReleaseEvent(event);
    m_context->doneCurrent();
}

void Canvas::mouseDoubleClickEvent(QMouseEvent * event)
{
    if (!m_navigation)
        return;

    m_context->makeCurrent(this);
    m_navigation->mouseDoubleClickEvent(event);
    m_context->doneCurrent();
}

void Canvas::wheelEvent(QWheelEvent * event)
{
    if (!m_navigation)
        return;

    m_context->makeCurrent(this);
    m_navigation->wheelEvent(event);
    m_context->doneCurrent();
}
