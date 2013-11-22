#include "Mammut.h"

#include <glm/gtx/transform.hpp>
#include <btBulletDynamicsCommon.h>

#include "Conversions.h"


Mammut::Mammut(btDiscreteDynamicsWorld * dynamicsWorld, const glm::vec3 & size, glm::vec3 translationVector)
:   m_dynamicsWorld(dynamicsWorld),
    m_size(size)
{
    m_modelMatrix = glm::translate(translationVector);

    btCollisionShape * shape = new btBoxShape(btVector3(size.x/2.0, size.y/2.0, size.z/2.0));
    btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), 
        btVector3(translationVector.x, translationVector.y, translationVector.z)));

    btScalar mass = 1;
    btVector3 fallInertia(0, 0, 0);
    shape->calculateLocalInertia(mass, fallInertia);

    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, motionState, shape, fallInertia);
    m_rigidBody = new btRigidBody(rigidBodyCI);
    // add first impulse to mammut
    m_rigidBody->setLinearVelocity(btVector3(0, 0, -5));
    m_rigidBody->setDamping(0.4, 0.0);
    m_rigidBody->setFriction(0.9);
    m_rigidBody->setAngularFactor(btVector3(0,0,0));
    // m_rigidBody->setLinearFactor(btVector3(0,0,1));

    m_dynamicsWorld->addRigidBody(m_rigidBody);
}

Mammut::~Mammut()
{
}

const glm::vec3 & Mammut::size() const
{
    return m_size;
}

const glm::mat4 & Mammut::modelMatrix() const
{
    return m_modelMatrix;
}

void Mammut::setModelMatrix(const glm::mat4 & matrix)
{
    m_modelMatrix = matrix;
}

void Mammut::update()
{
    btTransform transform;
    m_rigidBody->getMotionState()->getWorldTransform(transform);

    glm::mat4 mat;
    mat *= Conversions::toGlmMat4(transform.getOrigin());
    mat *= Conversions::toGlmMat4(transform.getRotation());
    this->setModelMatrix(mat);

    btVector3 velocity = m_rigidBody->getLinearVelocity();
    m_rigidBody->setLinearVelocity(btVector3(velocity.x(), velocity.y(), -5));
}

const glm::vec3 Mammut::position() const
{
    btTransform transform;
    m_rigidBody->getMotionState()->getWorldTransform(transform);
    return Conversions::toGlmVec3(transform.getOrigin());
}

const glm::mat4 Mammut::rotation() const
{
    btTransform transform;
    m_rigidBody->getMotionState()->getWorldTransform(transform);
    return Conversions::toGlmMat4(transform.getRotation());
}

void Mammut::setGravity(int gravity)
{
    btTransform transform;
    m_rigidBody->getMotionState()->getWorldTransform(transform);
    transform.setRotation(btQuaternion(0.0f, 0.0f, -3.14 / 2 * gravity - 3.14));
    m_rigidBody->setCenterOfMassTransform(transform);
}