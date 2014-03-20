#include "Cuboid.h"

#include <glm/gtx/transform.hpp>
#include <glowutils/AxisAlignedBoundingBox.h>
#include <btBulletDynamicsCommon.h>

#include <Util.h>


Cuboid::Cuboid(const glm::vec3 & size, const glm::vec3 & translation)
:   m_size(size)
,   m_containsBoost(false)
{    
    initializeRigidBody(size, translation);
}

Cuboid::~Cuboid()
{
}

glm::mat4 Cuboid::modelTransform() const
{
    return glm::translate(position()) * glm::scale(m_size);
}

glowutils::AxisAlignedBoundingBox Cuboid::boundingBox() const
{
    glowutils::AxisAlignedBoundingBox boundingBox;
    btVector3 llf, urb;
    
    m_rigidBody->getAabb(llf, urb);
    boundingBox.extend(Util::toGlmVec3(llf));
    boundingBox.extend(Util::toGlmVec3(urb));
    
    return boundingBox;
}

void Cuboid::setBoost()
{
    m_containsBoost = true;
}

bool Cuboid::containsBoost() const
{
    return m_containsBoost;
}

void Cuboid::collectBoost() const
{
    assert(containsBoost());
    m_containsBoost = false;
}

void Cuboid::initializeRigidBody(const glm::vec3 & size, const glm::vec3 & translation)
{
    m_collisionShape.reset(new btBoxShape(Util::toBtVec3(size / 2.0f)));
    
    m_motionState.reset(new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1),
                                                             Util::toBtVec3(translation))));
    
    btRigidBody::btRigidBodyConstructionInfo info(0, m_motionState.get(), m_collisionShape.get());
    
    info.m_restitution = 0.0f;
    
    m_rigidBody.reset(new btRigidBody(info));
    m_rigidBody->setUserPointer(this);
}
