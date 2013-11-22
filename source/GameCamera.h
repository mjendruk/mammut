#pragma once

#include <glm/glm.hpp>

class Mammoth;

class GameCamera
{
public:
    GameCamera(Mammoth & mammoth);

    virtual ~GameCamera();

    glm::vec3 panVector();
    void setPanVector(glm::vec3 pan);
    void modifyPanVector(glm::vec3 panDelta);

    void pan();
    void update();

    const glm::vec3 eye() const;
    const glm::vec3 center() const;
    const glm::vec3 up() const;

protected:
    glm::vec3 m_eye;
    glm::vec3 m_center;
    glm::vec3 m_up;

    glm::vec3 m_panVector;

    Mammoth & m_mammoth;
};