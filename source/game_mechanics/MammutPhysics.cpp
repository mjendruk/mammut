#include "MammutPhysics.h"

#include <cassert>
#include <QList>
#include <QDebug>
#include <glm/gtx/transform.hpp>
#include <btBulletDynamicsCommon.h>
#include <Util.h>
#include "Mammut.h"

MammutPhysics::MammutPhysics(const glm::vec3 & size,
    const glm::vec3 & translation, Mammut * mammut)
:   m_mammut(mammut)
{
    initializeRigidBody(size, translation);
}

MammutPhysics::~MammutPhysics()
{

}

void MammutPhysics::collisionEvent(const PhysicsObject & object,
    const btVector3 & collisionNormal)
{
    m_mammut->collisionEvent(object, Util::toGlmVec3(collisionNormal));
}

btRigidBody * MammutPhysics::rigidBody() const
{
    return m_rigidBody.get();
}

void MammutPhysics::clearForcesAndApplyGravity()
{
    m_rigidBody->clearForces();
    m_rigidBody->applyGravity();
}

void MammutPhysics::applyForce(const glm::vec3 & force)
{
    m_rigidBody->applyCentralForce(Util::toBtVec3(force));
}

void MammutPhysics::rotate(const glm::mat3 & rotation)
{
    btTransform transform;
    m_motionState->getWorldTransform(transform);

    transform.setBasis(Util::toBtMat3(rotation));
    m_rigidBody->setCenterOfMassTransform(transform);
}

void MammutPhysics::setVelocity(const glm::vec3 & velocity)
{
    m_rigidBody->setLinearVelocity(Util::toBtVec3(velocity));
}

glm::vec3 MammutPhysics::position() const
{
    btTransform transform;
    m_motionState->getWorldTransform(transform);
    
    return Util::toGlmVec3(transform.getOrigin());
}

glm::mat4 MammutPhysics::rotation() const
{
    btTransform transform;
    m_motionState->getWorldTransform(transform);
    
    return Util::toGlmMat4(transform.getRotation());
}

glm::vec3 MammutPhysics::velocity() const
{
    return Util::toGlmVec3(m_rigidBody->getLinearVelocity());
}

void MammutPhysics::initializeRigidBody(const glm::vec3 & size,
    const glm::vec3 & translation)
{
    m_collisionShape.reset(new btBoxShape(Util::toBtVec3(size / 2.0f)));
    m_motionState.reset(new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1),
                                                             Util::toBtVec3(translation))));

    const btScalar mass = 2.0f;
    btVector3 fallInertia;
    m_collisionShape->calculateLocalInertia(mass, fallInertia);

    btRigidBody::btRigidBodyConstructionInfo info(mass,
                                                  m_motionState.get(),
                                                  m_collisionShape.get(),
                                                  fallInertia);
    info.m_linearDamping = 0.05f;
    info.m_restitution = 0.0f;
    info.m_friction = 0.1f;
    
    m_rigidBody.reset(new btRigidBody(info));
    m_rigidBody->setUserPointer(this);
    m_rigidBody->setAngularFactor(btVector3(0,0,0));
}
