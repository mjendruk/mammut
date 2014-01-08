#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

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
    
    void rotate(const glm::mat3 & rotation);
    
    void limitVelocity();
    void steerRight();
    void steerLeft();
    void doNotSteer();
    
    void collidesWith(const GameObject & object,
                      const glm::mat3 & gravityTransform);
    void resetCollisionState();
    
    void applySteering(const glm::mat3 & gravityTransform);
    void applyForces();
    
    virtual glm::mat4 modelTransform() const;
    virtual glowutils::AxisAlignedBoundingBox boundingBox() const;
    
    bool isOnObject() const;
    
    const glm::vec3 & position() const;
    void setPosition(const glm::vec3 & position);
    
    const glm::mat4 & rotation() const;
    void setRotation(const glm::mat4 & rotation);
    
    float velocity() const;

protected:
    bool isOnObject(const GameObject & object,
                    const glm::mat3 & gravityTransform);
    
protected:
    glm::vec3 m_position;
    glm::mat4 m_rotation;
    glm::mat4 m_scaleTransform;
    
    float m_steering;
    glm::vec3 m_nonDriftForce;
    
    bool m_isOnObject;

    std::unique_ptr<btRigidBody> m_rigidBody;
    std::unique_ptr<MammutMotionState> m_motionState;
    std::unique_ptr<btCollisionShape> m_collisionShape;
    
    btDiscreteDynamicsWorld & m_dynamicsWorld;
};
