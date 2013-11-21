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

    glm::vec3 panVector();
    void setPanVector(glm::vec3 pan);
    void modifyPanVector(glm::vec3 panDelta);

    void pan();

protected:
    virtual void changed() const;

    Canvas * m_canvas;
    glm::vec3 m_panVector;
};