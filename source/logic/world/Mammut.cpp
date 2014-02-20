#include "Mammut.h"

#include <glm/gtx/transform.hpp>

#include <Util.h>
#include <sound/Sound.h>
#include "GameMechanics.h"

const glm::vec3 Mammut::s_size = glm::vec3(0.1f);

Mammut::Mammut(const glm::vec3 & translation)
:   m_physics(s_size, translation, this)
,   m_isOnObject(false)
,   m_isCrashed(false)
{
}

Mammut::~Mammut()
{
}

void Mammut::update()
{
    if (collidesWithCave())
        crash();

    if (!isStillOnObject()) {
        m_isOnObject = false;
        return;
    }
    
    m_physics.clearForcesAndApplyGravity();
    slowDownDrifting();
    
    const glm::vec3 forwardForce = glm::vec3(0.0f, 0.0f, -27.0f);
    m_physics.applyForce(forwardForce);
}

void Mammut::gravityChangeEvent(const glm::mat3 & rotation)
{
    m_gravityTransform = glm::inverse(rotation);
}

void Mammut::collisionEvent(const PhysicsObject & object,
    const glm::vec3 & collisionNormal)
{
    glm::vec3 rotatedAbsoluteNormal = glm::abs(m_gravityTransform * collisionNormal);
    
    switch (Util::maxAxis(rotatedAbsoluteNormal))
    {
        case Util::kXAxis:
            break;
        case Util::kYAxis:
            m_isOnObject = true;
            break;
        case Util::kZAxis:
            crash();
            break;
    }
}

float Mammut::caveDistanceRatio() const
{
    return glm::length(m_physics.position().xy()) / GameMechanics::s_caveRadius;
}

glm::mat4 Mammut::modelTransform() const
{
    return glm::translate(position()) * rotation() * glm::scale(s_size);
}

glm::vec3 Mammut::position() const
{
    return m_physics.position();
}

glm::vec3 Mammut::velocity() const
{
    return m_physics.velocity();
}

glm::mat4 Mammut::rotation() const
{
    return m_physics.rotation();
}

MammutPhysics * Mammut::physics()
{
    return &m_physics;
}

void Mammut::slowDownDrifting()
{
    glm::vec3 rotatedVelocity = m_gravityTransform * m_physics.velocity();
    rotatedVelocity = glm::vec3(rotatedVelocity.x * 0.9f, rotatedVelocity.yz());
    m_physics.setVelocity(glm::inverse(m_gravityTransform) * rotatedVelocity);
}

void Mammut::crash()
{
    if (m_isCrashed)
        return;

    Sound sound(Sound::kImpact);
    emit crashed();
    m_isCrashed = true;
}

bool Mammut::isStillOnObject() const
{
    if (!m_isOnObject)
        return false;

    const float epsilon = 0.01f;
    
    glm::vec3 velocity = m_gravityTransform * m_physics.velocity();
    return fabs(velocity.y) < epsilon;
}

bool Mammut::collidesWithCave() const
{
    return glm::length(m_physics.position().xy()) >= GameMechanics::s_caveRadius;
}
