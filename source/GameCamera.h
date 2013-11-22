#pragma once

#include <glm/glm.hpp>

class Mammoth;

class GameCamera
{
public:
    GameCamera(Mammoth & mammoth);

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

    Mammoth & m_mammoth;
};