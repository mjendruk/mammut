#include "Mammut.h"

#include <cmath>

#include <glm/gtx/transform.hpp>
#include <btBulletDynamicsCommon.h>

#include "Conversions.h"
#include "MammutMotionState.h"

Mammut::Mammut(const glm::vec3 translation, btDiscreteDynamicsWorld & dynamicsWorld)
:   m_dynamicsWorld(dynamicsWorld)
{
    const glm::vec3 size(0.1f);

    m_position = translation;
    m_scaleTransform = glm::scale(size);

    m_collisionShape.reset(new btBoxShape(Conversions::toBtVec3(size) / 2));
    m_motionState.reset(new MammutMotionState(translation, *this));

    btScalar mass = 1.0f;
    btVector3 fallInertia(0, 0, 0);
    m_collisionShape->calculateLocalInertia(mass, fallInertia);

    btRigidBody::btRigidBodyConstructionInfo info(mass, m_motionState.get(), m_collisionShape.get(), fallInertia);
    m_rigidBody.reset(new btRigidBody(info));
    m_rigidBody->setUserPointer(this);

    m_rigidBody->setFriction(1.5);
    m_rigidBody->setAngularFactor(btVector3(0,0,0));
    m_rigidBody->applyCentralForce(btVector3(0,0, -15));

    m_dynamicsWorld.addRigidBody(m_rigidBody.get());
}

Mammut::~Mammut()
{
    m_dynamicsWorld.removeRigidBody(m_rigidBody.get());
}

glm::mat4 Mammut::modelTransform() const
{
    return glm::translate(m_position) * m_rotation * m_scaleTransform;
}

const glm::vec3 & Mammut::position() const
{
    return m_position;
}

const glm::mat4 & Mammut::rotation() const
{
    return m_rotation;
}

void Mammut::setPosition(const glm::vec3 & position)
{
    m_position = position;
}

void Mammut::setRotation(const glm::mat4 & rotation)
{
    m_rotation = rotation;
}

void Mammut::update()
{
    m_rigidBody->applyCentralForce(btVector3(0,0, -15));
}

void Mammut::changeGravity(Gravity direction)
{
    btTransform transform;
    m_motionState->getWorldTransform(transform);
    transform.setRotation(btQuaternion(0.0f, 0.0f, -M_PI / 2 * direction));
    m_rigidBody->setCenterOfMassTransform(transform);
}
