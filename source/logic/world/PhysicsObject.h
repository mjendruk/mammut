#pragma once

#include <memory>

#include <glm/glm.hpp>

class btRigidBody; 
class btCollisionShape;
class btMotionState;
class btVector3;

class PhysicsObject
{
public:
    virtual ~PhysicsObject();

    virtual void collisionEvent(const PhysicsObject & object,
                                const btVector3 & collisionNormal);

    btRigidBody * rigidBody() const;

    glm::vec3 position() const; 
    glm::mat4 rotation() const;
    glm::vec3 velocity() const;

    void addZShift(float zShift);

    virtual bool containsBoost() const;
    virtual void collectBoost() const;

protected:
    std::unique_ptr<btCollisionShape> m_collisionShape;
    std::unique_ptr<btMotionState> m_motionState;
    std::unique_ptr<btRigidBody> m_rigidBody;
};
