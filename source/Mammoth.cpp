#include "Mammoth.h"

#include <glm/gtx/transform.hpp>
#include <btBulletDynamicsCommon.h>

#include <QDebug>


Mammoth::Mammoth(btDiscreteDynamicsWorld * dynamicsWorld, const glm::vec3 & size, glm::vec3 translationVector)
:   m_dynamicsWorld(dynamicsWorld),
    m_size(size)
{
    m_modelMatrix = glm::translate(translationVector);

    btCollisionShape * shape = new btBoxShape(btVector3(size.x, size.y, size.z));
    btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), 
        btVector3(translationVector.x, translationVector.y, translationVector.z)));

    btScalar mass = 1;
    btVector3 fallInertia(0, 0, 0);
    shape->calculateLocalInertia(mass, fallInertia);

    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, motionState, shape, fallInertia);
    m_rigidBody = new btRigidBody(rigidBodyCI);
    //add first impulse to mammoth
    m_rigidBody->setLinearVelocity(btVector3(0, 0, -10));

    m_dynamicsWorld->addRigidBody(m_rigidBody);
}

Mammoth::~Mammoth()
{
}

const glm::vec3 & Mammoth::size() const
{
    return m_size;
}

const glm::mat4 & Mammoth::modelMatrix() const
{
    return m_modelMatrix;
}

void Mammoth::setModelMatrix(const glm::mat4 & matrix)
{
    m_modelMatrix = matrix;
}

void Mammoth::updatePhysics()
{
    btTransform transform;
    m_rigidBody->getMotionState()->getWorldTransform(transform);
    btVector3 origin = transform.getOrigin();
    btQuaternion quat = transform.getRotation();

    glm::mat4 mat;
    mat *= glm::translate(origin.x(), origin.y(), origin.z());
    mat *= glm::rotate(glm::degrees(quat.getAngle()), glm::vec3(quat.getAxis().x(), quat.getAxis().y(), quat.getAxis().z()));
    this->setModelMatrix(mat);
}

const glm::vec3 Mammoth::position() const
{
    btTransform transform;
    m_rigidBody->getMotionState()->getWorldTransform(transform);
    btVector3 origin = transform.getOrigin();
    return glm::vec3(origin.x(), origin.y(), origin.z());
}


const glm::mat4 Mammoth::rotation() const
{
    btTransform transform;
    m_rigidBody->getMotionState()->getWorldTransform(transform);
    btQuaternion quaternion = transform.getRotation();
    return glm::rotate(glm::degrees(quaternion.getAngle()), quaternion.getAxis().x(), quaternion.getAxis().y(), quaternion.getAxis().z());
}