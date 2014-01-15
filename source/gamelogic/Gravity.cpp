#include "Gravity.h"

#include <cmath>

#include <QDebug>

#include <glm/gtx/transform.hpp>
#include <btBulletDynamicsCommon.h>

#include <Conversions.h>

Gravity::Gravity(btDiscreteDynamicsWorld & dynamicsWorld)
:   m_state(kDown)
,   m_dynamicsWorld(dynamicsWorld)
{
    rotate(kDown);
}

Gravity::~Gravity()
{

}

void Gravity::rotate(Direction direction)
{
    m_state = static_cast<Direction>((m_state + direction) % 4);
    
    float angle = 90 * m_state;
    m_rotation = glm::mat3(glm::rotate(angle, glm::vec3(0.0f, 0.0f, 1.0f)));
    m_inverseRotation = glm::mat3(glm::rotate(-angle, glm::vec3(0.0f, 0.0f, 1.0f)));
    
    const glm::vec3 gravityAcceleration(0.0f, -15.81f, 0.0f);
    m_dynamicsWorld.setGravity(Conversions::toBtVec3(m_rotation * gravityAcceleration));
}

const glm::mat3 & Gravity::rotation() const
{
    return m_rotation;
}

const glm::mat3 & Gravity::inverseRotation() const
{
    return m_inverseRotation;
}
