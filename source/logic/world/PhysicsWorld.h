#pragma once

#include <QObject>
#include <glm/glm.hpp>
#include <btBulletDynamicsCommon.h>

class PhysicsObject;
class MammutPhysics;

class PhysicsWorld : public QObject
{
    Q_OBJECT

public:
    enum GravityDirection { kGravityDown, kGravityRight, kGravityUp, kGravityLeft };
    
    PhysicsWorld();
    virtual ~PhysicsWorld();

    void addObject(PhysicsObject * object);
    void removeObject(PhysicsObject * object);
    void setMammutPhysics(MammutPhysics * mammutPhysics);
    
    void changeGravity(GravityDirection direction);

    void stepSimulation(float seconds);
    void preTickCallback(float timeStep);
    void postTickCallback(float timeStep);

signals:
    void simulationTick(float seconds);
    void gravityChanged(const glm::mat3 & rotation);

protected:
    static const glm::vec3 s_defaultGravity;
    static const glm::vec3 s_mammutGravity;
    btDbvtBroadphase m_broadphase;
    btDefaultCollisionConfiguration m_collisionConfiguration;
    btCollisionDispatcher m_dispatcher;
    btSequentialImpulseConstraintSolver m_solver;
    btDiscreteDynamicsWorld m_dynamicsWorld;
    
    GravityDirection m_gravity;

    MammutPhysics * m_mammutPhysics;
};
