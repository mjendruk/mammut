#pragma once

#include <QScopedPointer>
#include <QSharedPointer>
#include <QList>

#include <glm/glm.hpp>
#include <glowutils/AxisAlignedBoundingBox.h>

#include "GameObject.h"

class btCollisionShape;
class btRigidBody;
class btMotionState;
class btDiscreteDynamicsWorld;

class Cuboid : public GameObject
{
public:
    Cuboid(const glm::vec3 & size,
           const glm::vec3 & translation,
           btDiscreteDynamicsWorld & dynamicsWorld);
         
    ~Cuboid();

    const glm::mat4 & modelTransform() const;
    const glm::vec3 & position() const;
    const glm::vec3 & size() const;
    
    const glowutils::AxisAlignedBoundingBox & boundingBox() const;

protected:
    void initializeRigidBody(const glm::vec3 & size, const glm::vec3 & translation);
    void initializeBoundingBox(const glm::vec3 & size, const glm::vec3 & translation);
    
    glm::mat4 m_modelTransform;
    glm::vec3 m_position;
    glm::vec3 m_size;
    
    glowutils::AxisAlignedBoundingBox m_boundingBox;
    
    QScopedPointer<btRigidBody> m_rigidBody;
    QScopedPointer<btMotionState> m_motionState;
    QScopedPointer<btCollisionShape> m_collisionShape;
    
    btDiscreteDynamicsWorld & m_dynamicsWorld;
};
