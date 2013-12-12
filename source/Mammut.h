#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Commons.h"
#include "GameObject.h"

class btCollisionShape;
class btRigidBody;
class btDiscreteDynamicsWorld;

class MammutMotionState;

class Mammut : public GameObject
{
public:
    Mammut(const glm::vec3 translation,
           btDiscreteDynamicsWorld & dynamicsWorld);
         
    ~Mammut();

    void update();
    void changeGravity(Gravity gravity);
    
    glm::mat4 modelTransform() const;
    
    const glm::vec3 & position() const;
    const glm::mat4 & rotation() const;
    
    void setPosition(const glm::vec3 & position);
    void setRotation(const glm::mat4 & rotation);

protected:
    glm::vec3 m_position;
    glm::mat4 m_rotation;
    glm::mat4 m_scaleTransform;

    std::unique_ptr<btRigidBody> m_rigidBody;
    std::unique_ptr<MammutMotionState> m_motionState;
    std::unique_ptr<btCollisionShape> m_collisionShape;
    
    btDiscreteDynamicsWorld & m_dynamicsWorld;
};
