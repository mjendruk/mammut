#include "Mammut.h"

#include <QDebug>

#include <glm/gtx/transform.hpp>

#include <btBulletDynamicsCommon.h>

#include <Util.h>
#include <sound/Sound.h>
#include "Cuboid.h"

const glm::vec3 Mammut::s_size = glm::vec3(0.1f);
const int Mammut::s_maxBoosts = 5;
const float Mammut::s_boostVelocity = 1.f;
const float Mammut::s_boostDistance = 5.f;

Mammut::Mammut(const glm::vec3 & translation)
:   m_physics(s_size, translation, this)
,   m_isOnObject(false)
,   m_isCrashed(false)
,   m_boostIsActive(false)
,   m_collectedBoosts(5)
{
}

Mammut::~Mammut()
{
}

void Mammut::update()
{
    updateBoost();

    // in air
    if (!isStillOnObject()) {
        m_isOnObject = false;
        return;
    }
    
    //on cuboid
    m_physics.clearForcesAndApplyGravity();
    if (!m_boostIsActive)
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
    if (m_boostIsActive)
        assert(false); //TODO -> return?

    if (m_collectedBoosts < 1)
        return;

    float magnitude = 1.f; //different magnitude in gravity direction? or in -gravity
    glm::vec3 boostForce;   // magnitude <> speed?

    switch (direction)
    {
    case BoostDirection::kUp:
        boostForce = glm::vec3(0.f, magnitude, 0.f);
        break;
    case BoostDirection::kRight:
        boostForce = glm::vec3(magnitude, 0.f, 0.f);
        break;
    case BoostDirection::kDown:
        if (isStillOnObject())
            return;
        boostForce = glm::vec3(0.f, -magnitude, 0.f);
        break;
    case BoostDirection::kLeft:
        boostForce = glm::vec3(-magnitude, 0.f, 0.f);
        break;
    }

    m_boostDirection = boostForce;
    m_boostIsActive = true;
    --m_collectedBoosts;

    qDebug() << "start Boost: " << m_boostDirection.x << " | " << m_boostDirection.y;

    m_numBoostSteps = 20;
    updateBoost();
}

void Mammut::updateBoost()
{
    if (!m_boostIsActive)
        return;

    if (m_numBoostSteps <= 0){
        m_boostIsActive = false;
        return;
    } 

    btTransform mammutTransform = m_physics.rigidBody()->getCenterOfMassTransform();

    float currentLength = 0.7f; //s(m) = t(s) * vel (m/s)
    glm::vec3 shiftVector = (glm::inverse(m_gravityTransform) * m_boostDirection) * currentLength;

    glm::vec3 position = Util::toGlmVec3(m_physics.rigidBody()->getCenterOfMassTransform().getOrigin());
    btVector3 newPosition = Util::toBtVec3(position + shiftVector);

    mammutTransform.setOrigin(newPosition);
    m_physics.rigidBody()->setCenterOfMassTransform(mammutTransform);

    --m_numBoostSteps;

    qDebug() << "apply Boost: " << shiftVector.x << " | " << shiftVector.y;
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
