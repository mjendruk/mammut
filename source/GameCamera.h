#pragma once

#include <glm/glm.hpp>

class Mammut;

class GameCamera
{
public:
    GameCamera(Mammut & mammut);

    virtual ~GameCamera();

    void update();

    const glm::vec3 eye() const;
    const glm::vec3 center() const;
    const glm::vec3 up() const;

protected:
    static const glm::vec3 s_offset;
    glm::vec3 m_eye;
    glm::vec3 m_center;
    glm::vec3 m_up;

    glm::vec3 m_panVector;

    Mammut & m_mammut;
};