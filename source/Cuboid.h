#pragma once

#include <memory>
#include <glm/glm.hpp>

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

protected:
    glm::mat4 m_modelTransform;

    std::unique_ptr<btRigidBody> m_rigidBody;
    std::unique_ptr<btMotionState> m_motionState;
    std::unique_ptr<btCollisionShape> m_collisionShape;
    
    btDiscreteDynamicsWorld & m_dynamicsWorld;
};
