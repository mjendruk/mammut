
#pragma once

#include <glm/glm.hpp>

class btDiscreteDynamicsWorld;

class Gravity
{
public:
    enum Direction { kDown, kRight, kUp, kLeft };

    Gravity(btDiscreteDynamicsWorld & dynamicsWorld);
    virtual ~Gravity();

    void rotate(Direction direction);

    const glm::mat3 & rotation() const;
    const glm::mat3 & inverseRotation() const;

protected:
    Direction m_state;
    glm::mat3 m_rotation;
    glm::mat3 m_inverseRotation;

    btDiscreteDynamicsWorld & m_dynamicsWorld;
};
