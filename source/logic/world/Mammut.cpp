#include "Mammut.h"

#include <glm/gtx/transform.hpp>

#include <Util.h>
#include <sound/Sound.h>
#include "Cuboid.h"

const glm::vec3 Mammut::s_size = glm::vec3(0.1f);
const int Mammut::s_maxBoosts = 5;

Mammut::Mammut(const glm::vec3 & translation)
:   m_physics(s_size, translation, this)
,   m_isOnObject(false)
,   m_isCrashed(false)
,   m_collectedBoosts(0)
{
}

Mammut::~Mammut()
{
}

void Mammut::update()
{
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
            if (object.containsBoost()) 
            {
                object.collectBoost();
                addBoost();
            }
            m_isOnObject = true;
            break;
        case Util::kZAxis:
            crash();
            break;
    }
}

void Mammut::caveCollisionEvent()
{
    crash();
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

int Mammut::boosts() const
{
    return m_collectedBoosts;
}

void Mammut::addBoost()
{
    if (m_collectedBoosts < s_maxBoosts)
        ++m_collectedBoosts;
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
