#include "Canvas.h"

#include <cassert>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#endif

#include <QCursor>
#include <QDebug>
#include <QResizeEvent>
#include <QScreen>

#include <glow/global.h>
#include <glow/logging.h>
#include <glow/Version.h>
#include <glow/Texture.h>
#include <glow/FrameBufferObject.h>

#include <Util.h>
#include "Renderer.h"

namespace
{
    
void checkVersion()
{
    glow::info("Version %;", glow::version().toString());
    glow::info("Vendor: %;", glow::vendor());
    glow::info("Renderer %;", glow::renderer());
    glow::info("Core profile: %;", glow::isCoreProfile() ? "true" : "false");
    glow::info("GLSL version: %;", glow::getString(GL_SHADING_LANGUAGE_VERSION));
    glow::info("GL Versionstring: %;\n", glow::versionString());
}
    
} // namespace

Canvas::Canvas(const QSurfaceFormat & format)
:   QWindow((QScreen*)nullptr)
,   m_renderer(nullptr)
,   m_swapInterval(VerticalSyncronization)
#ifdef __APPLE__
,   m_isFullscreen(false)
#endif
{
    setSurfaceType(OpenGLSurface);
    create();

    initializeGL(format);
    initializeAppearance();
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

void Canvas::initializeAppearance()
{
    resize(QSize(800, 600));
    setMinimumSize(size());
    
    QSize position = screen()->virtualSize();
    position = position / 2 - size() / 2;
    setFramePosition(QPoint(position.width(), position.height()));
    
    setCursor(QCursor(Qt::BlankCursor));
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

    if (!glow::init(true))
    {
        return;
    }

    checkVersion();

    initializeScreenshotFbo();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void Canvas::initializeScreenshotFbo()
{
    m_screenshotFbo = new glow::FrameBufferObject();
    m_screenshotDepthAttachment = Util::create2DTexture(GL_NEAREST, GL_CLAMP_TO_EDGE);
    m_screenshotFbo->attachTexture2D(GL_DEPTH_ATTACHMENT, m_screenshotDepthAttachment);
}

void Canvas::resizeEvent(QResizeEvent * event)
{
    const int width = event->size().width();
    const int height = event->size().height();

    if(m_renderer != nullptr)
        m_renderer->resize(width, height);

    m_screenshotDepthAttachment->image2D(0, GL_DEPTH_COMPONENT24,
                                         width * devicePixelRatio(),
                                         height * devicePixelRatio(), 0,
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

    glow::Texture * texture = Util::create2DTexture(GL_NEAREST, GL_CLAMP_TO_EDGE);
    texture->image2D(0, GL_RGBA8,
                     width() * devicePixelRatio(),
                     height() * devicePixelRatio(),
                     0, GL_RGBA, GL_FLOAT, nullptr);

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

    CGLContextObj contextObj;
    GLint swapIntervalParam = swapInterval;
    
    m_context.makeCurrent(this);
    contextObj = CGLGetCurrentContext();

    CGLError error = CGLSetParameter(contextObj, kCGLCPSwapInterval, &swapIntervalParam);
    result = (error == kCGLNoError);

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
    default:
        return QString();
    }
}

void Canvas::showFullscreen()
{
#ifdef __APPLE__
    m_windowedFlags = flags();
    m_windowedGeometry = geometry();
    
    setFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setGeometry(screen()->geometry());
    
    requestActivate();
#else
    showFullScreen();
#endif
    
    m_isFullscreen = true;
}

void Canvas::showWindowed()
{
#ifdef __APPLE__
    setFlags(Qt::Window);
    
    if (isFullscreen())
    {
        setFlags(m_windowedFlags);
        setGeometry(m_windowedGeometry);
    }
    
    requestActivate();
#else
    showNormal();
#endif
    
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
