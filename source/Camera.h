#pragma once

#include <glowutils/Camera.h>

class Canvas;


class Camera : public glow::Camera
{
public:
    Camera(const glm::vec3 & eye = glm::vec3(0.0, 0.0, 1.0), 
           const glm::vec3 & center = glm::vec3(0.0, 0.0, 0.0),
           const glm::vec3 & up = glm::vec3(0.0, 1.0, 0.0));

    virtual ~Camera();

    void setCanvas(Canvas * canvas);

protected:
    virtual void changed() const;

    Canvas * m_canvas;
};