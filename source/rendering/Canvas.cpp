#include "Canvas.h"

#include <cassert>

#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QResizeEvent>

#include <glow/Texture.h>
#include <glow/FrameBufferObject.h>

#include "Renderer.h"

Canvas::Canvas(const QSurfaceFormat & format)
:   QWindow((QScreen*)nullptr)
,   m_renderer(nullptr)
,   m_swapInterval(VerticalSyncronization)
,   m_swapts(0.0)
,   m_swaps(0)
,   m_isFullscreen(false)
{
    setSurfaceType(OpenGLSurface); 
    create();

    initializeGL(format);
    showWindowed();
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

    initializeScreenshotFbo();
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void Canvas::initializeScreenshotFbo()
{
    m_screenshotFbo = new glow::FrameBufferObject();
    
    m_screenshotDepthAttachment = new glow::Texture(GL_TEXTURE_2D);
    m_screenshotDepthAttachment->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    m_screenshotDepthAttachment->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    m_screenshotDepthAttachment->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    m_screenshotDepthAttachment->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    m_screenshotDepthAttachment->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    m_screenshotFbo->attachTexture2D(GL_DEPTH_ATTACHMENT, m_screenshotDepthAttachment);
}

void Canvas::resizeEvent(QResizeEvent * event)
{
    const int width = event->size().width();
    const int height = event->size().height();
    
    if(m_renderer != nullptr)
        m_renderer->resize(width, height);

    m_screenshotDepthAttachment->image2D(0, GL_DEPTH_COMPONENT24, width * devicePixelRatio(), height * devicePixelRatio(), 0,
                                         GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    
    if (isExposed())
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_context.swapBuffers(this);
    }
}

void Canvas::render()
{
    assert(m_renderer != nullptr);
    
    if (!isExposed() || Hidden == visibility() || Minimized == visibility())
        return;
    
    m_context.makeCurrent(this);
    
    m_renderer->render(glow::FrameBufferObject::defaultFBO(), devicePixelRatio());
    
    m_context.swapBuffers(this);
}

glow::Texture * Canvas::screenshot()
{
    assert(m_renderer != nullptr);
    
    glow::Texture * texture = new glow::Texture(GL_TEXTURE_2D);
    texture->image2D(0, GL_RGBA32F,
                     width() * devicePixelRatio(),
                     height() * devicePixelRatio(),
                     0, GL_RGBA, GL_FLOAT, nullptr);
    
    texture->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    texture->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    texture->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    texture->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    texture->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    m_screenshotFbo->attachTexture2D(GL_COLOR_ATTACHMENT0, texture);
    m_screenshotFbo->setDrawBuffers({ GL_COLOR_ATTACHMENT0 });
    
    m_renderer->render(m_screenshotFbo, devicePixelRatio());
    
    return texture;
}

void Canvas::setRenderer(Renderer * renderer)
{
    assert(renderer != nullptr);
    
    renderer->resize(width(), height());
    
    m_renderer = renderer;
}

void Canvas::setSwapInterval(SwapInterval swapInterval)
{
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

void Canvas::showFullscreen()
{
    setFlags(Qt::CustomizeWindowHint |
             Qt::FramelessWindowHint |
             Qt::WindowStaysOnTopHint );
    
    m_windowedRect = geometry();
    QRect rect = QApplication::desktop()->screenGeometry(QApplication::desktop()->primaryScreen());
    setGeometry(rect);
    requestActivate();
    
    m_isFullscreen = true;
}

void Canvas::showWindowed()
{
    if (isFullscreen())
        setGeometry(m_windowedRect);
    
    setFlags(Qt::Window);
    
    requestActivate();
    m_isFullscreen = false;
}

void Canvas::toggleFullscreen()
{
    isFullscreen() ? showWindowed() : showFullscreen();
}

bool Canvas::isFullscreen() const
{
    return m_isFullscreen;
}