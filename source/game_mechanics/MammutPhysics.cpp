#include "MammutPhysics.h"

#include <cassert>
#include <QList>
#include <QDebug>
#include <glm/gtx/transform.hpp>
#include <btBulletDynamicsCommon.h>
#include <Conversions.h>
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
    m_mammut->collisionEvent(object, Conversions::toGlmVec3(collisionNormal));
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

void MammutPhysics::applyForwardForce(float force)
{
    btVector3 forceVec(0.0f, 0.0f, -force);
    m_rigidBody->applyCentralForce(forceVec);
}

void MammutPhysics::rotate(const glm::mat3 & rotation)
{
    btTransform transform;
    m_motionState->getWorldTransform(transform);

    transform.setBasis(Conversions::toBtMat3(rotation));
    m_rigidBody->setCenterOfMassTransform(transform);
}

glm::vec3 MammutPhysics::position() const
{
    btTransform transform;
    m_motionState->getWorldTransform(transform);
    
    return Conversions::toGlmVec3(transform.getOrigin());
}

glm::mat4 MammutPhysics::rotation() const
{
    btTransform transform;
    m_motionState->getWorldTransform(transform);
    
    return Conversions::toGlmMat4(transform.getRotation());
}

glm::vec3 MammutPhysics::velocity() const
{
    return Conversions::toGlmVec3(m_rigidBody->getLinearVelocity());
}

void MammutPhysics::initializeRigidBody(const glm::vec3 & size,
    const glm::vec3 & translation)
{
    m_collisionShape.reset(new btBoxShape(Conversions::toBtVec3(size / 2.0f)));
    m_motionState.reset(new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1),
                                                             Conversions::toBtVec3(translation))));

    const btScalar mass = 0.9f;
    btVector3 fallInertia;
    m_collisionShape->calculateLocalInertia(mass, fallInertia);

    btRigidBody::btRigidBodyConstructionInfo info(mass,
                                                  m_motionState.get(),
                                                  m_collisionShape.get(),
                                                  fallInertia);
    info.m_friction = 0.8f;
    info.m_linearDamping = 0.05f;
    
    m_rigidBody.reset(new btRigidBody(info));
    m_rigidBody->setUserPointer(this);
    m_rigidBody->setAngularFactor(btVector3(0,0,0));
}
