
#include "Canvas.h"
#include "Camera.h"

Camera::Camera(const glm::vec3 & eye, const glm::vec3 & center, 
    const glm::vec3 & up)
:    glow::Camera(eye, center, up)
,    m_canvas(nullptr)
{
}

Camera::~Camera()
{
    
}

void Camera::changed() const
{
    if (!m_canvas)
        return;

    m_canvas->cameraChanged();
}

void Camera::setCanvas(Canvas *canvas)    
{
    m_canvas = canvas;
}