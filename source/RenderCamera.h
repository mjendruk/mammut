#pragma once

#include <glowutils/Camera.h>

class Canvas;


class RenderCamera : public glow::Camera
{
public:
    RenderCamera(const glm::vec3 & eye = glm::vec3(0.0, 0.0, 1.0),
           const glm::vec3 & center = glm::vec3(0.0, 0.0, 0.0),
           const glm::vec3 & up = glm::vec3(0.0, 1.0, 0.0));

    virtual ~RenderCamera();

    void setCanvas(Canvas * canvas);
    void pan(glm::vec3 panVector);

protected:
    virtual void changed() const;

    Canvas * m_canvas;
};