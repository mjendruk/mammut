#include "RenderCamera.h"

#include "Canvas.h"

RenderCamera::RenderCamera(const glm::vec3 & eye, const glm::vec3 & center,
    const glm::vec3 & up)
    : glow::Camera(eye, center, up),
    m_canvas(nullptr),
    m_panVector(glm::vec3(0.f))
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

void RenderCamera::pan()
{
    setEye(m_panVector + m_eye);
    setCenter(m_panVector + m_center);
    update();
}

void RenderCamera::setPanVector(glm::vec3 pan)
{
    m_panVector = pan;
}

void RenderCamera::modifyPanVector(glm::vec3 panDelta)
{
    m_panVector += panDelta;
}

glm::vec3 RenderCamera::panVector()
{
    return m_panVector;
}
