#include "Mammut.h"

#include <glm/gtx/transform.hpp>

#include <Util.h>
#include <sound/Sound.h>
#include "Cuboid.h"

const glm::vec3 Mammut::s_size = glm::vec3(0.1f);
const int Mammut::s_maxNumBoosts = 5;
const double Mammut::s_startIncreasingSpeed = 100.f;
const double Mammut::s_stopIncreasingSpeed = 1500.f;

Mammut::Mammut(const glm::vec3 & translation)
:   m_physics(s_size, translation, this)
,   m_isOnObject(false)
,   m_isCrashed(false)
,   m_boostIsActive(false)
,   m_collectedBoosts(0)
,   m_zDistance(0.0)
{
}

Mammut::~Mammut()
{
}

void Mammut::update()
{
    updateBoostState();

    if (!isStillOnObject()) {
        m_isOnObject = false;
        return;
    }

    m_physics.clearForcesAndApplyGravity();

    if (!m_boostIsActive)
        slowDownDrifting();

    float forwardIntensity = glm::smoothstep(s_startIncreasingSpeed, s_stopIncreasingSpeed, m_zDistance + m_physics.position().z) * 15.0 + 15.0; // [15, 30] 
    const glm::vec3 forwardForce = glm::vec3(0.0f, 0.0f, -forwardIntensity);
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
        collectBoostFromObject(object);
        break;
    case Util::kYAxis:
        collectBoostFromObject(object);
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

int Mammut::collectedBoosts() const
{
    return m_collectedBoosts;
}

void Mammut::collectBoostFromObject(const PhysicsObject & object)
{
    if (object.containsBoost())
    {
        object.collectBoost();
        if (m_collectedBoosts < s_maxNumBoosts)
            ++m_collectedBoosts;
    }
}

void Mammut::applyBoost(BoostDirection direction)
{
    if (m_collectedBoosts < 1)
        return;

    float magnitude = 15.f;
    glm::vec3 boostVector;

    switch (direction)
    {
    case BoostDirection::kUp:
        boostVector = glm::vec3(0.f, magnitude, 0.f);
        break;
    case BoostDirection::kRight:
        boostVector = glm::vec3(magnitude, 0.f, 0.f);
        break;
    case BoostDirection::kDown:
        if (isStillOnObject())
            return;
        boostVector = glm::vec3(0.f, -magnitude, 0.f);
        break;
    case BoostDirection::kLeft:
        boostVector = glm::vec3(-magnitude, 0.f, 0.f);
        break;
    }

    glm::vec3 mammutVelocity = m_physics.velocity();
    float length = std::max(glm::length(mammutVelocity), 15.f) * 0.05f;
    glm::vec3 boostWithGravityTransform = glm::inverse(m_gravityTransform) * boostVector;
    glm::vec3 resultVelocity = mammutVelocity + boostWithGravityTransform * length;
    m_physics.setVelocity(resultVelocity);

    m_boostIsActive = true;
    --m_collectedBoosts;
    physicSteps = 80;
    
    Sound sound(Sound::kBeam);
}

void Mammut::updateBoostState()
{
    if (!m_boostIsActive)
        return;

    --physicSteps;

    if (physicSteps <= 0)
        m_boostIsActive = false;
}

void Mammut::addZShift(float zShift)
{
    m_zDistance -= m_physics.position().z;
    m_physics.addZShift(zShift);
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
