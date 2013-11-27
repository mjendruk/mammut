#include "Cuboid.h"

#include <glm/gtx/transform.hpp>
#include <btBulletDynamicsCommon.h>

#include "Conversions.h"


Cuboid::Cuboid(const glm::vec3 & size, const glm::vec3 & translation,
    btDiscreteDynamicsWorld & dynamicsWorld)
:    m_dynamicsWorld(dynamicsWorld)
{
    m_modelTransform = glm::translate(translation) * glm::scale(size);
    m_collisionShape.reset(new btBoxShape(btVector3(size.x / 2.0f,
    
                                                    size.y / 2.0f,
                                                    size.z / 2.0f)));
    
    m_motionState.reset(new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1),
                                                             Conversions::toBtVec3(translation))));

    btRigidBody::btRigidBodyConstructionInfo info(0, m_motionState.get(), m_collisionShape.get());
    m_rigidBody.reset(new btRigidBody(info));
    
    m_dynamicsWorld.addRigidBody(m_rigidBody.get());
}

Cuboid::~Cuboid()
{
}

const glm::mat4 & Cuboid::modelTransform() const
{
    return m_modelTransform;
}
