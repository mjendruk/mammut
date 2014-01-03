#pragma once

#include <GL/glew.h>

#include <QWindow>
#include <QScopedPointer>
#include <QOpenGLContext>

class QSurfaceFormat;

class Renderer;
class RenderCamera;

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
    Canvas(const QSurfaceFormat & format, Renderer * renderer, RenderCamera * camera);
    virtual ~Canvas();

    // from QWindow
    virtual QSurfaceFormat format() const;

    void setSwapInterval(SwapInterval swapInterval);
    static const QString swapIntervalToString(SwapInterval swapInterval);

    void beginPaintGL();
    void endPaintGL();


public slots:
    void toggleSwapInterval();

protected:
    const QString querys(const GLenum penum);
    const GLint queryi(const GLenum penum);

    virtual void initializeGL(const QSurfaceFormat & format);

    //virtual void showEvent(QShowEvent * event);
    //virtual void hideEvent(QHideEvent * event);

    virtual void resizeEvent(QResizeEvent * event);

protected:
    QOpenGLContext m_context;

    SwapInterval m_swapInterval;    ///< required for toggle

    Renderer * m_renderer;
    RenderCamera * m_camera;

    long double m_swapts;
    unsigned int m_swaps;
};
