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
