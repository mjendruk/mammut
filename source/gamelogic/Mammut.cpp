#include "Mammut.h"

#include <cmath>
#include <iostream>

#include <QDebug>

#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glowutils/AxisAlignedBoundingBox.h>

#include <btBulletDynamicsCommon.h>

#include "../Conversions.h"
#include "../GlowExtensions.h"
#include "MammutMotionState.h"

Mammut::Mammut(const glm::vec3 translation, btDiscreteDynamicsWorld & dynamicsWorld)
:   m_steering(0.0f)
,   m_isOnObject(false)
,   m_dynamicsWorld(dynamicsWorld)
{
    const glm::vec3 size(0.1f);

    m_position = translation;
    m_scaleTransform = glm::scale(size);

    m_collisionShape.reset(new btBoxShape(Conversions::toBtVec3(size) / 2));
    m_motionState.reset(new MammutMotionState(translation, *this));

    btScalar mass = 0.9f;
    btVector3 fallInertia(0, 0, 0);
    m_collisionShape->calculateLocalInertia(mass, fallInertia);

    btRigidBody::btRigidBodyConstructionInfo info(mass,
                                                  m_motionState.get(),
                                                  m_collisionShape.get(),
                                                  fallInertia);
    info.m_friction = 0.5f;
    info.m_linearDamping = 0.05f;
    
    m_rigidBody.reset(new btRigidBody(info));
    m_rigidBody->setUserPointer(this);
    m_rigidBody->setAngularFactor(btVector3(0,0,0));

    m_dynamicsWorld.addRigidBody(m_rigidBody.get());
}

Mammut::~Mammut()
{
    m_dynamicsWorld.removeRigidBody(m_rigidBody.get());
}

void Mammut::rotate(const glm::mat3 & rotation)
{
    btTransform transform;
    m_motionState->getWorldTransform(transform);
    glm::vec3 impulse = rotation * glm::vec3(0.0f, -3.0f, 0.0f);
    m_rigidBody->applyCentralImpulse(Conversions::toBtVec3(impulse));
    transform.setBasis(Conversions::toBtMat3(rotation));
    m_rigidBody->setCenterOfMassTransform(transform);
}

void Mammut::applyForces()
{
    if (!m_isOnObject)
        return;
    
    m_rigidBody->clearForces();
    
    const glm::vec3 forwardForce(0.0f, 0.0f, -27.0f);
    
    m_rigidBody->applyCentralForce(Conversions::toBtVec3(forwardForce));
}

void Mammut::limitVelocity()
{
    btVector3 velocity = m_rigidBody->getLinearVelocity();

    static const float maxVelocity = -70.0f;
    if (velocity.z() < maxVelocity)
    {
        velocity.setZ(maxVelocity);
        m_rigidBody->setLinearVelocity(velocity);
    }
    
}

void Mammut::steerRight()
{
    m_steering = 12.0f;
}

void Mammut::steerLeft()
{
    m_steering = -12.0f;
}
                    
void Mammut::doNotSteer()
{
    m_steering = 0.0f;
}

void Mammut::applySteering(const glm::mat3 & gravityTransform)
{
    if (!m_isOnObject)
        return;
    
    glm::vec3 steering = gravityTransform * glm::vec3(m_steering, 0.0f, 0.0f);
    glm::vec3 velocity = gravityTransform * Conversions::toGlmVec3(m_rigidBody->getLinearVelocity());
    
    glm::vec3 newVelocity = glm::inverse(gravityTransform) * glm::vec3(steering.x, velocity.yz);
    
    m_rigidBody->setLinearVelocity(Conversions::toBtVec3(newVelocity));
}

void Mammut::collidesWith(const GameObject & object, const glm::mat3 & gravityTransform)
{
    if (isOnObject(object, gravityTransform))
    {
        glm::vec3 force = Conversions::toGlmVec3(m_rigidBody->getTotalForce());
//        m_nonDriftForce = glm::vec3(-(gravityTransform * force).x, 0.0f, 0.0f);
        
        m_isOnObject = true;
    }
}

void Mammut::resetCollisionState()
{
    m_isOnObject = false;
    m_nonDriftForce = glm::vec3(0.0f);
}

glm::mat4 Mammut::modelTransform() const
{
    return glm::translate(m_position) * m_rotation * m_scaleTransform;
}

glowutils::AxisAlignedBoundingBox Mammut::boundingBox() const
{
    glowutils::AxisAlignedBoundingBox boundingBox;
    btVector3 llf, urb;
    
    m_rigidBody->getAabb(llf, urb);
    boundingBox.extend(Conversions::toGlmVec3(llf));
    boundingBox.extend(Conversions::toGlmVec3(urb));

    return boundingBox;
}

bool Mammut::isOnObject() const
{
    return m_isOnObject;
}

const glm::vec3 & Mammut::position() const
{
    return m_position;
}

void Mammut::setPosition(const glm::vec3 & position)
{
    m_position = position;
}

const glm::mat4 & Mammut::rotation() const
{
    return m_rotation;
}

void Mammut::setRotation(const glm::mat4 & rotation)
{
    m_rotation = rotation;
}
        
bool Mammut::isOnObject(const GameObject & object, const glm::mat3 & gravityTransform)
{
    const float epsilon = 0.001f;
    
    float objectTop = GlowExtensions::transformedBoundingBox(object.boundingBox(),
                                                           glm::mat4(gravityTransform)).urb().y;
    float mammutBottom = GlowExtensions::transformedBoundingBox(boundingBox(),
                                                              glm::mat4(gravityTransform)).llf().y;
    
    return fabs(objectTop - mammutBottom) < epsilon;
}

float Mammut::velocity() const
{
    return -m_rigidBody->getLinearVelocity().z();
}
