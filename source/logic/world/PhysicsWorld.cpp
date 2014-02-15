#include "PhysicsWorld.h"

#include <glm/gtx/transform.hpp>

#include <Util.h>
#include "PhysicsObject.h"

namespace
{

void forwardPreTickCallback(btDynamicsWorld * world, btScalar timeStep)
{
    PhysicsWorld * physicsWorld = static_cast<PhysicsWorld *>(world->getWorldUserInfo());
    physicsWorld->preTickCallback(timeStep);
}
    
void forwardPostTickCallback(btDynamicsWorld * world, btScalar timeStep)
{
    PhysicsWorld * physicsWorld = static_cast<PhysicsWorld *>(world->getWorldUserInfo());
    physicsWorld->postTickCallback(timeStep);
}

} // namespace

PhysicsWorld::PhysicsWorld()
:   m_dispatcher(&m_collisionConfiguration)
,   m_dynamicsWorld(&m_dispatcher, 
                    &m_broadphase, 
                    &m_solver, 
                    &m_collisionConfiguration)
,   m_gravity(kGravityDown)
{
    btContactSolverInfo& info = m_dynamicsWorld.getSolverInfo();
    info.m_splitImpulse = 1;
    info.m_splitImpulsePenetrationThreshold = -0.02f;
    
    m_dynamicsWorld.setInternalTickCallback(forwardPreTickCallback, this, true);
    m_dynamicsWorld.setInternalTickCallback(forwardPostTickCallback, this, false);
    
    changeGravity(kGravityDown);
}

PhysicsWorld::~PhysicsWorld()
{
}

void PhysicsWorld::stepSimulation(float seconds)
{
    m_dynamicsWorld.stepSimulation(seconds, 25, 0.002f);
}

void PhysicsWorld::changeGravity(GravityDirection direction)
{
    const glm::vec3 gravityAcceleration(0.0f, -30.0f, 0.0f);
    
    m_gravity = static_cast<GravityDirection>((m_gravity + direction) % 4);
    
    const float angle = 90.0f * m_gravity;
    const glm::mat3 rotation = glm::mat3(glm::rotate(angle, glm::vec3(0.0f, 0.0f, 1.0f)));
    
    m_dynamicsWorld.setGravity(Util::toBtVec3(rotation * gravityAcceleration));
    
    emit gravityChanged(rotation);
}

void PhysicsWorld::addObject(PhysicsObject * object)
{
    m_dynamicsWorld.addRigidBody(object->rigidBody());
}

void PhysicsWorld::removeObject(PhysicsObject * object)
{
    m_dynamicsWorld.removeRigidBody(object->rigidBody());
}

void PhysicsWorld::preTickCallback(float timeStep)
{
    emit simulationTick(timeStep);
}

void PhysicsWorld::postTickCallback(float timeStep)
{
    int manifoldsCount = m_dispatcher.getNumManifolds();
    for (int i = 0; i < manifoldsCount; i++)
    {
        btPersistentManifold * manifold = m_dispatcher.getManifoldByIndexInternal(i);

        if (manifold->getNumContacts() == 0)
            continue;

        const btCollisionObject * body0 = manifold->getBody0();
        const btCollisionObject * body1 = manifold->getBody1();
        
        PhysicsObject * obj1 = static_cast<PhysicsObject *>(body0->getUserPointer());
        PhysicsObject * obj2 = static_cast<PhysicsObject *>(body1->getUserPointer());

        btVector3 normalOnB = manifold->getContactPoint(0).m_normalWorldOnB;
        
        obj1->collisionEvent(*obj2, -normalOnB);
        obj2->collisionEvent(*obj1, normalOnB);
    }
}
