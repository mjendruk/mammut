#include "PhysicsObject.h"

#include <btBulletDynamicsCommon.h>

#include <Util.h>

PhysicsObject::~PhysicsObject()
{
}

void PhysicsObject::collisionEvent(const PhysicsObject & object, 
    const btVector3 & collisionNormal)
{
}

btRigidBody * PhysicsObject::rigidBody() const
{
    return m_rigidBody.get();
}

glm::vec3 PhysicsObject::position() const
{
    btTransform transform;
    m_motionState->getWorldTransform(transform);

    return Util::toGlmVec3(transform.getOrigin());
}

void PhysicsObject::addZShift(float zShift)
{
    btTransform transform;
    m_motionState->getWorldTransform(transform);
    transform.setOrigin(transform.getOrigin() + btVector3(0.0f, 0.0f, zShift));
    m_motionState->setWorldTransform(transform);

    m_rigidBody->translate(btVector3(0.0f, 0.0f, zShift));
}

void PhysicsObject::translate(const glm::vec3 & translation)
{
    btVector3 btTranslation = Util::toBtVec3(translation);
    btTransform transform;
    m_motionState->getWorldTransform(transform);
    transform.setOrigin(transform.getOrigin() + btTranslation);
    m_motionState->setWorldTransform(transform);

    m_rigidBody->translate(btTranslation);
}

glm::mat4 PhysicsObject::rotation() const
{
    btTransform transform;
    m_motionState->getWorldTransform(transform);

    return Util::toGlmMat4(transform.getRotation());
}

glm::vec3 PhysicsObject::velocity() const
{
    return Util::toGlmVec3(m_rigidBody->getLinearVelocity());
}

bool PhysicsObject::containsBoost() const
{
    return false;
}

void PhysicsObject::collectBoost() const
{
}
