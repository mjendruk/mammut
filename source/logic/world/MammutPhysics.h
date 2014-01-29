#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "PhysicsObject.h"

class btCollisionShape;
class btMotionState;
class btRigidBody;

class Mammut;

class MammutPhysics : public PhysicsObject
{
public:
    MammutPhysics(const glm::vec3 & size,
                  const glm::vec3 & translation,
                  Mammut * mammut);
    
    virtual ~MammutPhysics();

    void collisionEvent(const PhysicsObject & object,
                        const btVector3 & collisionNormal) override;

    btRigidBody * rigidBody() const override;
    
    void clearForcesAndApplyGravity();
    void applyForce(const glm::vec3 & force);
    void rotate(const glm::mat3 & rotation);
    
    void setVelocity(const glm::vec3 & velocity);
    
    glm::vec3 position() const;
    glm::mat4 rotation() const;
    glm::vec3 velocity() const;

protected:
    void initializeRigidBody(const glm::vec3 & size,
                             const glm::vec3 & translation);

protected:
    std::unique_ptr<btCollisionShape> m_collisionShape;
    std::unique_ptr<btMotionState> m_motionState;
    std::unique_ptr<btRigidBody> m_rigidBody;

    Mammut * m_mammut;
};
