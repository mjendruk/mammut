#include "Cuboid.h"

#include <QDebug>
#include <glm/gtx/transform.hpp>
#include <btBulletDynamicsCommon.h>

#include "../Conversions.h"


Cuboid::Cuboid(const glm::vec3 & size, const glm::vec3 & translation,
    btDiscreteDynamicsWorld & dynamicsWorld)
:    m_dynamicsWorld(dynamicsWorld)
{
    m_position = translation;
    m_size = size;
    m_modelTransform = glm::translate(translation) * glm::scale(size);
    
    initializeRigidBody(size, translation);
    initializeBoundingBox();
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

void Cuboid::initializeBoundingBox()
{
    btVector3 llf, urb;
    m_rigidBody->getAabb(llf, urb);
    m_boundingBox.extend(Conversions::toGlmVec3(llf));
    m_boundingBox.extend(Conversions::toGlmVec3(urb));
}

glm::mat4 Cuboid::modelTransform() const
{
    return m_modelTransform;
}

glowutils::AxisAlignedBoundingBox Cuboid::boundingBox() const
{
    return m_boundingBox;
}

const glm::vec3 & Cuboid::position() const
{
    return m_position;
}

const glm::vec3 & Cuboid::size() const
{
    return m_size;
}
