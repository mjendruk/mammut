#pragma once

#include <glm/glm.hpp>

#include "PhysicsObject.h"

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
    
    void clearForcesAndApplyGravity();
    void applyForce(const glm::vec3 & force);
    
    void setVelocity(const glm::vec3 & velocity);

protected:
    void initializeRigidBody(const glm::vec3 & size,
                             const glm::vec3 & translation);

protected:

    Mammut * m_mammut;
};
