#include "Canvas.h"

#include <QDebug>
#include <QApplication>
#include <QResizeEvent>
#include <QOpenGLContext>

Canvas::Canvas(const QSurfaceFormat & format, GameLogic & gameLogic)
:   QWindow((QScreen*)nullptr)
,   m_renderer(gameLogic, *this)
,   m_swapInterval(VerticalSyncronization)
,   m_swapts(0.0)
,   m_swaps(0)
{
    setSurfaceType(OpenGLSurface); 
    create();

    initializeGL(format);
    
    m_context.makeCurrent(this);
    m_renderer.initialize();
    m_context.doneCurrent();
}

Canvas::~Canvas()
{
}

QSurfaceFormat Canvas::format() const
{
    return m_context.format();
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
    m_context.setFormat(format);
    if (!m_context.create())
    {
        qCritical() << "Errors during creation of OpenGL context.";
        return;
    }

    m_context.makeCurrent(this);
    
    glewExperimental = GL_TRUE;
    if (!(glewInit() == GLEW_OK))
    {
        qCritical() << "Initializing GLEW failed.";
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
    
    glClearColor(1.f, 1.f, 1.f, 0.f);

    m_context.doneCurrent();
}

void Canvas::resizeEvent(QResizeEvent * event)
{
    const int width = event->size().width();
    const int height = event->size().height();
    
    m_context.makeCurrent(this);
    
    m_renderer.resize(width, height);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_context.swapBuffers(this);
    
    m_context.doneCurrent();
}

void Canvas::render()
{
    if (!isExposed() || Hidden == visibility())
        return;
    
    m_context.makeCurrent(this);
    
    m_renderer.paint();
    
    m_context.swapBuffers(this);
    m_context.doneCurrent();
}

void Canvas::setSwapInterval(SwapInterval swapInterval)
{
    m_context.makeCurrent(this);

    bool result(false);
    m_swapInterval = swapInterval;

#ifdef WIN32

    // ToDo: C++11 - type aliases
    typedef bool(WINAPI * SWAPINTERVALEXTPROC) (int);
    static SWAPINTERVALEXTPROC wglSwapIntervalEXT = nullptr;

    if (!wglSwapIntervalEXT)
        wglSwapIntervalEXT = reinterpret_cast<SWAPINTERVALEXTPROC>(m_context.getProcAddress("wglSwapIntervalEXT"));
    if (wglSwapIntervalEXT)
        result = wglSwapIntervalEXT(swapInterval);

#elif __APPLE__

    qWarning("ToDo: Setting swap interval is currently not implemented for __APPLE__");

#else
    // ToDo: C++11 - type aliases
    typedef int(APIENTRY * SWAPINTERVALEXTPROC) (int);
    static SWAPINTERVALEXTPROC glXSwapIntervalSGI = nullptr;

    if (!glXSwapIntervalSGI)
        glXSwapIntervalSGI = reinterpret_cast<SWAPINTERVALEXTPROC>(m_context.getProcAddress("glXSwapIntervalSGI"));
    if (glXSwapIntervalSGI)
        result = glXSwapIntervalSGI(swapInterval);

#endif

    if (!result)
        qWarning("Setting swap interval to %s failed."
            , qPrintable(swapIntervalToString(swapInterval)));
    else
        qDebug("Setting swap interval to %s."
            , qPrintable(swapIntervalToString(swapInterval)));

    m_context.doneCurrent();
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
