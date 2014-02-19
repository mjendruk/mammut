#pragma once

#include <memory>
#include <glm/glm.hpp>

#include "PhysicsObject.h"

namespace glowutils
{
    class AxisAlignedBoundingBox;
}

class btMotionState;
class btCollisionShape;

class Cuboid : public PhysicsObject
{
public:
    Cuboid(const glm::vec3 & size,
           const glm::vec3 & translation);
         
    virtual ~Cuboid();

    const glm::mat4 & modelTransform() const;

    glowutils::AxisAlignedBoundingBox boundingBox() const;
    
    btRigidBody * rigidBody() const override;

protected:
    void initializeRigidBody(const glm::vec3 & size, const glm::vec3 & translation);
    void initializeModelTransform();

protected:
    std::unique_ptr<btCollisionShape> m_collisionShape;
    std::unique_ptr<btMotionState> m_motionState;
    std::unique_ptr<btRigidBody> m_rigidBody;

    glm::mat4 m_modelTransform;
    glm::vec3 m_size;
};
