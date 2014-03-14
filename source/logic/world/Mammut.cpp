#include "Mammut.h"

#include <QDebug>

#include <glm/gtx/transform.hpp>

#include <btBulletDynamicsCommon.h>

#include <Util.h>
#include <sound/Sound.h>
#include "Cuboid.h"

const glm::vec3 Mammut::s_size = glm::vec3(0.1f);
const int Mammut::s_maxBoosts = 5;

Mammut::Mammut(const glm::vec3 & translation)
:   m_physics(s_size, translation, this)
,   m_isOnObject(false)
,   m_isCrashed(false)
,   m_boostIsActive(false)
,   m_collectedBoosts(1)
{
}

Mammut::~Mammut()
{
}

void Mammut::update()
{
    // in air
    if (!isStillOnObject()) {
        updateBoost(false);
        m_isOnObject = false;
        return;
    }
    
    //on cuboid
    m_physics.clearForcesAndApplyGravity();
    if (!m_boostIsActive)
        slowDownDrifting();
    
    updateBoost(true);

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

int Mammut::collectedBoosts() const
{
    return m_collectedBoosts;
}

void Mammut::addBoost()
{
    if (m_collectedBoosts < s_maxBoosts)
        ++m_collectedBoosts;
}

void Mammut::applyBoost(BoostDirection direction)
{
    if (m_collectedBoosts < 1)
        return;

    float magnitude = 800.f; //different magnitude in gravity direction? or in -gravity
    glm::vec3 boostForce;   // magnitude <> speed?
    float gravity = 27.f;
    float factor = 1.f;

    if (!isStillOnObject() && (direction == BoostDirection::kUp || direction == BoostDirection::kDown)) {
        glm::vec3 velocity = Util::toGlmVec3(m_physics.rigidBody()->getLinearVelocity());
        velocity = m_gravityTransform * velocity;
        gravity = velocity.y;
    }

    switch (direction)
    {
    case BoostDirection::kUp:
        factor = isStillOnObject() ? 1.3 : glm::abs(gravity / 27.f) * 2.f;
        boostForce = glm::vec3(0.f, magnitude * factor, 0.f);
        break;
    case BoostDirection::kRight:
        boostForce = glm::vec3(magnitude, 0.f, 0.f);
        break;
    case BoostDirection::kDown:
        factor = glm::abs(27.f / gravity);
        boostForce = glm::vec3(0.f, -magnitude * factor, 0.f);
        break;
    case BoostDirection::kLeft:
        boostForce = glm::vec3(-magnitude, 0.f, 0.f);
        break;
    }

    m_boostVector = boostForce;
    --m_collectedBoosts;
    m_boostIsActive = true;
    
    qDebug() << "start Boost: " << m_boostVector.x << " | " << m_boostVector.y;

    updateBoost(true);
}

void Mammut::updateBoost(bool applyPhysicalForce)
{
    if (!m_boostIsActive)
        return;

    if (m_boostIsActive && glm::abs(m_boostVector.x) < 10.f && glm::abs(m_boostVector.y) < 10.f){
        qDebug() << "end boost";
        m_boostIsActive = false;
        m_boostVector = glm::vec3(0.f);
        return;
    }

    if (applyPhysicalForce) //apply Boost again, all forces were cleared
        m_physics.applyForce(glm::inverse(m_gravityTransform) * m_boostVector);

    m_boostVector *= glm::vec3(0.8f, 0.8f, 0.f);

    qDebug() << "apply Boost: " << m_boostVector.x << " | " << m_boostVector.y;
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
