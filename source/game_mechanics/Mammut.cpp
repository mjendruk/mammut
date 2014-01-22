#include "Mammut.h"

#include <cmath>
#include <iostream>

#include <QDebug>

#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glowutils/AxisAlignedBoundingBox.h>

#include <btBulletDynamicsCommon.h>

#include <Conversions.h>

namespace
{
    
int maxAxis(const glm::vec3 & vec)
{
    return vec.x < vec.y ? (vec.y < vec.z ? 2 : 1) : (vec.x < vec.z ? 2 : 0);
}
    
} // namespace

const glm::vec3 Mammut::s_size = glm::vec3(0.1f);

Mammut::Mammut(const glm::vec3 & translation)
:   m_physics(s_size, translation, this)
,   m_isOnObject(false)
{
    const glm::vec3 size(0.1f);
}

Mammut::~Mammut()
{
}

void Mammut::update()
{
    if (!m_isOnObject)
        return;

    if (!isStillOnObject()) {
        m_isOnObject = false;
        return;
    }
    
    const float forwardForce = 27.0f;
    m_physics.clearForcesAndApplyGravity();
    m_physics.applyForwardForce(forwardForce);
}

void Mammut::gravityChangeEvent(const glm::mat3 & rotation)
{
    m_physics.rotate(rotation);
    m_gravityTransform = glm::inverse(rotation);
}

void Mammut::collisionEvent(const PhysicsObject & object,
    const glm::vec3 & collisionNormal)
{
    enum Axes { kXAxis, kYAxis, kZAxis };
    
    glm::vec3 rotatedAbsoluteNormal = glm::abs(m_gravityTransform * collisionNormal);
    
    switch (maxAxis(rotatedAbsoluteNormal))
    {
        case kYAxis:
            m_isOnObject = true;
            break;
        case kZAxis:
            emit crashed();
            break;
    }
}

glm::mat4 Mammut::modelTransform() const
{
    return glm::translate(position()) * rotation() * glm::scale(s_size);
}

glm::vec3 Mammut::position() const
{
    return m_physics.position();
}

glm::mat4 Mammut::rotation() const
{
    return m_physics.rotation();
}

float Mammut::velocity() const
{
    return -m_physics.velocity().z;
}

MammutPhysics * Mammut::physics()
{
    return &m_physics;
}

bool Mammut::isStillOnObject() const
{
    const float epsilon = 0.01f;
    
    glm::vec3 velocity = m_gravityTransform * m_physics.velocity();
    return fabs(velocity.y) < epsilon;
}



