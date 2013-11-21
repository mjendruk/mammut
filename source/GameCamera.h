#pragma once

#include <glm/glm.hpp>

class GameCamera
{
public:
    GameCamera(const glm::vec3 & eye = glm::vec3(0.0, 0.0, 1.0),
           const glm::vec3 & center = glm::vec3(0.0, 0.0, 0.0),
           const glm::vec3 & up = glm::vec3(0.0, 1.0, 0.0));

    virtual ~GameCamera();

    glm::vec3 panVector();
    void setPanVector(glm::vec3 pan);
    void modifyPanVector(glm::vec3 panDelta);

    void pan();

    const glm::vec3 eye() const;
    const glm::vec3 center() const;
    const glm::vec3 up() const;

protected:
    glm::vec3 m_eye;
    glm::vec3 m_center;
    glm::vec3 m_up;

    glm::vec3 m_panVector;
};