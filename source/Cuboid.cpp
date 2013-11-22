#include "Cuboid.h"

#include <glm/gtx/transform.hpp>
#include <btBulletDynamicsCommon.h>


Cuboid::Cuboid(btDiscreteDynamicsWorld * dynamicsWorld, const glm::vec3 & size, glm::vec3 translationVector)
:   m_dynamicsWorld(dynamicsWorld),
    m_size(size)
{
    m_modelMatrix = glm::translate(translationVector);

    btCollisionShape * shape = new btBoxShape(btVector3(size.x / 2.0, size.y / 2.0, size.z / 2.0));
    btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), 
        btVector3(translationVector.x, translationVector.y, translationVector.z)));

    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(0, motionState, shape, btVector3(0, 0, 0));
    m_rigidBody = new btRigidBody(rigidBodyCI);

    m_dynamicsWorld->addRigidBody(m_rigidBody);
}

Cuboid::~Cuboid()
{
}

const glm::vec3 & Cuboid::size() const
{
    return m_size;
}

const glm::mat4 & Cuboid::modelMatrix() const
{
    return m_modelMatrix;
}

void Cuboid::setModelMatrix(const glm::mat4 & matrix)
{
    m_modelMatrix = matrix;
}

void Cuboid::update()
{
    btTransform transform;
    m_rigidBody->getMotionState()->getWorldTransform(transform);
    btVector3 origin = transform.getOrigin();
    btQuaternion quat = transform.getRotation();

    glm::mat4 mat;
    mat *= glm::translate(origin.x(), origin.y(), origin.z());
    mat *= glm::rotate(quat.getAngle(), quat.getAxis().x(), quat.getAxis().y(), quat.getAxis().z());
    this->setModelMatrix(mat);
}
