#include "Mammut.h"

#include <cmath>

#include <QDebug>

#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glowutils/AxisAlignedBoundingBox.h>

#include <btBulletDynamicsCommon.h>

#include "Conversions.h"
#include "GlowExtensions.h"
#include "MammutMotionState.h"

Mammut::Mammut(const glm::vec3 translation, btDiscreteDynamicsWorld & dynamicsWorld)
:   m_isOnObject(false)
,   m_dynamicsWorld(dynamicsWorld)
{
    const glm::vec3 size(0.1f);

    m_position = translation;
    m_scaleTransform = glm::scale(size);

    m_collisionShape.reset(new btBoxShape(Conversions::toBtVec3(size) / 2));
    m_motionState.reset(new MammutMotionState(translation, *this));

    btScalar mass = 1.0f;
    btVector3 fallInertia(0, 0, 0);
    m_collisionShape->calculateLocalInertia(mass, fallInertia);

    btRigidBody::btRigidBodyConstructionInfo info(mass,
                                                  m_motionState.get(),
                                                  m_collisionShape.get(),
                                                  fallInertia);
    m_rigidBody.reset(new btRigidBody(info));
    m_rigidBody->setUserPointer(this);

    m_rigidBody->setFriction(1.5);
    m_rigidBody->setAngularFactor(btVector3(0,0,0));
    m_rigidBody->applyCentralForce(btVector3(0,0, -15));

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
    transform.setBasis(Conversions::toBtMat3(rotation));
    m_rigidBody->setCenterOfMassTransform(transform);
}

void Mammut::applyForces()
{
    if (!m_isOnObject)
        return;
    
    const btVector3 force = btVector3(0.0f, 0.0f, -30.0f);
    m_rigidBody->applyCentralForce(force);
}

void Mammut::collidesWith(const GameObject & object, const glm::mat3 & gravityTransform)
{
    if (isOnObject(object, gravityTransform))
        m_isOnObject = true;
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

void Mammut::setIsOnObject(bool b)
{
    m_isOnObject = b;
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
