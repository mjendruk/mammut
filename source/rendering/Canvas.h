#pragma once

#include <functional>

#include <GL/glew.h>

#include <QWindow>
#include <QScopedPointer>
#include <QOpenGLContext>

#include <glow/ref_ptr.h>

namespace glow
{
    class FrameBufferObject;
    class Texture;
}

class Renderer;

class Canvas : public QWindow
{
    Q_OBJECT

public:
    enum SwapInterval
    {
        NoVerticalSyncronization        =  0
    ,   VerticalSyncronization          =  1 ///< WGL_EXT_swap_control, GLX_EXT_swap_control, GLX_SGI_video_sync
    ,   AdaptiveVerticalSyncronization  = -1 ///< requires EXT_swap_control_tear
    };

public:
    Canvas(const QSurfaceFormat & format);
    virtual ~Canvas();

    QSurfaceFormat format() const override;

    void setSwapInterval(SwapInterval swapInterval);
    static const QString swapIntervalToString(SwapInterval swapInterval);
    
    void render();
    glow::ref_ptr<glow::Texture> screenshot();

    void setRenderer(Renderer * renderer);

public slots:
    void toggleSwapInterval();

protected:
    const QString querys(const GLenum penum);
    const GLint queryi(const GLenum penum);

    void initializeGL(const QSurfaceFormat & format);
    void initializeScreenshotFbo();

    void resizeEvent(QResizeEvent * event) override;

protected:
    QOpenGLContext m_context;
    Renderer * m_renderer;

    glow::ref_ptr<glow::FrameBufferObject> m_screenshotFbo;
    glow::ref_ptr<glow::Texture> m_screenshotDepthAttachment;

    SwapInterval m_swapInterval;    ///< required for toggle
    long double m_swapts;
    unsigned int m_swaps;
};
