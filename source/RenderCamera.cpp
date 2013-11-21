#include "RenderCamera.h"

#include "Canvas.h"

RenderCamera::RenderCamera(const glm::vec3 & eye, const glm::vec3 & center,
    const glm::vec3 & up)
:   glow::Camera(eye, center, up),
    m_canvas(nullptr)
{
}

RenderCamera::~RenderCamera()
{
    
}

void RenderCamera::changed() const
{
    if (!m_canvas)
        return;

    m_canvas->cameraChanged();
}

void RenderCamera::setCanvas(Canvas * canvas)
{
    m_canvas = canvas;
}

void RenderCamera::pan(glm::vec3 panVector)
{
    setEye(panVector + m_eye);
    setCenter(panVector + m_center);
    update();
}