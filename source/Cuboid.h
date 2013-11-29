#pragma once

#include <QScopedPointer>
#include <QSharedPointer>
#include <QList>

#include <glm/glm.hpp>
#include <glowutils/AxisAlignedBoundingBox.h>

class btCollisionShape;
class btRigidBody;
class btMotionState;
class btDiscreteDynamicsWorld;

class Cuboid
{
public:
    Cuboid(const glm::vec3 & size,
           const glm::vec3 & translation,
           btDiscreteDynamicsWorld & dynamicsWorld);
         
    ~Cuboid();

    const glm::mat4 & modelTransform() const;
    const glow::AxisAlignedBoundingBox & boundingBox() const;

protected:
    void initializeRigidBody(const glm::vec3 & size, const glm::vec3 & translation);
    void initializeBoundingBox(const glm::vec3 & size, const glm::vec3 & translation);
    
    glm::mat4 m_modelTransform;
    glow::AxisAlignedBoundingBox m_boundingBox;
    
    QScopedPointer<btRigidBody> m_rigidBody;
    QScopedPointer<btMotionState> m_motionState;
    QScopedPointer<btCollisionShape> m_collisionShape;
    
    btDiscreteDynamicsWorld & m_dynamicsWorld;
};
