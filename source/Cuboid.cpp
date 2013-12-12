#include "Cuboid.h"

#include <QDebug>
#include <glm/gtx/transform.hpp>
#include <btBulletDynamicsCommon.h>

#include "Conversions.h"


Cuboid::Cuboid(const glm::vec3 & size, const glm::vec3 & translation,
    btDiscreteDynamicsWorld & dynamicsWorld)
:    m_dynamicsWorld(dynamicsWorld)
{
    m_modelTransform = glm::translate(translation) * glm::scale(size);
    
    initializeRigidBody(size, translation);
    initializeBoundingBox(size, translation);
}

Cuboid::~Cuboid()
{
    m_dynamicsWorld.removeRigidBody(m_rigidBody.data());
}

void Cuboid::initializeRigidBody(const glm::vec3 & size, const glm::vec3 & translation)
{
    m_collisionShape.reset(new btBoxShape(btVector3(size.x / 2.0f,
                                                    size.y / 2.0f,
                                                    size.z / 2.0f)));
    
    m_motionState.reset(new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1),
                                                             Conversions::toBtVec3(translation))));
    
    btRigidBody::btRigidBodyConstructionInfo info(0, m_motionState.data(), m_collisionShape.data());
    m_rigidBody.reset(new btRigidBody(info));
    m_rigidBody->setUserPointer(this);
    
    m_dynamicsWorld.addRigidBody(m_rigidBody.data());
}

void Cuboid::initializeBoundingBox(const glm::vec3 & size, const glm::vec3 & translation)
{
    glm::vec4 urb = m_modelTransform * glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec4 llf = m_modelTransform * glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f);
    m_boundingBox.extend(llf.xyz() / llf.w);
    m_boundingBox.extend(urb.xyz() / urb.w);
}

const glm::mat4 & Cuboid::modelTransform() const
{
    return m_modelTransform;
}

const glowutils::AxisAlignedBoundingBox & Cuboid::boundingBox() const
{
    return m_boundingBox;
}
