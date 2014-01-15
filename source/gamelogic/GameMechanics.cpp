#include "GameMechanics.h"

#include <cassert>

#include <QDebug>
#include <QKeyEvent>
#include <QVector>

#include <glm/gtx/transform.hpp>

#include <rendering/GameWorldRenderer.h>

#include "Cuboid.h"

namespace Callbacks
{
    
void preTickCallback(btDynamicsWorld * world, btScalar timeStep)
{
    GameMechanics * gameMechanics = static_cast<GameMechanics *>(world->getWorldUserInfo());
    gameMechanics->preTickCallback(timeStep);
}
    
void postTickCallback(btDynamicsWorld * world, btScalar timeStep)
{
    GameMechanics * gameMechanics = static_cast<GameMechanics *>(world->getWorldUserInfo());
    gameMechanics->postTickCallback(timeStep);
}

} // namespace

GameMechanics::GameMechanics()
:   m_dispatcher(&m_collisionConfiguration)
,   m_dynamicsWorld(&m_dispatcher, 
                    &m_broadphase, 
                    &m_solver, 
                    &m_collisionConfiguration)
,   m_chunkGenerator(1337, m_dynamicsWorld)
,   m_mammut(glm::vec3(0.0f, 0.7f, 4.5f), m_dynamicsWorld)
,   m_camera(m_mammut)
,   m_gravity(m_dynamicsWorld)
,   m_renderer(*this)
{
    registerTickCallbacks();
                    
    for (int i = 0; i < 10 ; i++)
        m_chunkList << m_chunkGenerator.nextChunk();
}

GameMechanics::~GameMechanics()
{
    
}

void GameMechanics::update(float seconds)
{
    m_dynamicsWorld.stepSimulation(seconds, 10, 0.005f);
    
    m_camera.update(seconds);
    
    if (m_chunkList.at(1)->boundingBox().llf().z > m_camera.center().z)
    {
        m_chunkList.removeFirst();
        m_chunkList << m_chunkGenerator.nextChunk();
    }
}

Renderer * GameMechanics::renderer()
{
    return &m_renderer;
}

void GameMechanics::keyPressed(QKeyEvent * event)
{
    switch (event->key())
    {
    case Qt::Key_W:
        m_gravity.rotate(Gravity::kUp);
        m_mammut.rotate(m_gravity.rotation());
        break;
    case Qt::Key_A:
        m_gravity.rotate(Gravity::kLeft);
        m_mammut.rotate(m_gravity.rotation());
        break;
    case Qt::Key_D:
        m_gravity.rotate(Gravity::kRight);
        m_mammut.rotate(m_gravity.rotation());
        break;
    case Qt::Key_Left:
        m_mammut.steerLeft();
        break;
    case Qt::Key_Right:
        m_mammut.steerRight();
        break;
    }
}

void GameMechanics::keyReleased(QKeyEvent * event)
{
    switch (event->key())
    {
    case Qt::Key_Left:
    case Qt::Key_Right:
        m_mammut.doNotSteer();
        break;
    }
}

void GameMechanics::registerTickCallbacks()
{   
    m_dynamicsWorld.setInternalTickCallback(Callbacks::preTickCallback,
                                             static_cast<void *>(this), true);
    m_dynamicsWorld.setInternalTickCallback(Callbacks::postTickCallback,
                                             static_cast<void *>(this), false);
}

void GameMechanics::preTickCallback(float timeStep)
{
    m_mammut.applyForces();
    m_mammut.resetCollisionState();
}

void GameMechanics::postTickCallback(float timeStep)
{
    m_mammut.limitVelocity();
    
    int manifoldsCount = m_dispatcher.getNumManifolds();
    for (int i = 0; i < manifoldsCount; i++)
    {
        btPersistentManifold * manifold = m_dispatcher.getManifoldByIndexInternal(i);
        const btCollisionObject * body0 = manifold->getBody0();
        const btCollisionObject * body1 = manifold->getBody1();
        
        GameObject * obj1 = static_cast<GameObject *>(body0->getUserPointer());
        GameObject * obj2 = static_cast<GameObject *>(body1->getUserPointer());
        
        Mammut * mammut = dynamic_cast<Mammut *>(obj1);
        Cuboid * cuboid = dynamic_cast<Cuboid *>(obj2);
        
        if (!mammut)
        {
            mammut = dynamic_cast<Mammut *>(obj2);
            cuboid = dynamic_cast<Cuboid *>(obj1);
        }
        
        assert(mammut != nullptr);
        assert(cuboid != nullptr);
        
        mammut->collidesWith(*cuboid, m_gravity.inverseRotation());
    }
    
    m_mammut.applySteering(m_gravity.inverseRotation());
}

const GameCamera & GameMechanics::camera() const
{
    return m_camera;
}

const Mammut & GameMechanics::mammut() const
{
    return m_mammut;
}

void GameMechanics::forEachCuboid(const std::function<void (Cuboid &)> & lambda)
{
    for (QSharedPointer<CuboidChunk> & chunk : m_chunkList)
        for (Cuboid * cuboid : chunk->cuboids())
            lambda(*cuboid);
}




