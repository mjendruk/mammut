#include "GameLogic.h"

#include <cassert>

#include <QDebug>
#include <QVector>

#include <glm/gtx/transform.hpp>
#include <btBulletDynamicsCommon.h>

#include "Cuboid.h"
#include "GameCamera.h"
#include "Mammut.h"

namespace
{
    
void tickCallback(btDynamicsWorld * world, btScalar timeStep)
{
    GameLogic * gameLogic = static_cast<GameLogic *>(world->getWorldUserInfo());
    gameLogic->simulationTickCallback(timeStep);
}

} // namespace

GameLogic::GameLogic()
:   m_activeGravity(kDown)
{
    initializeDynamicsWorld();
    
    m_dynamicsWorld->setInternalTickCallback(tickCallback, static_cast<void *>(this), true);
    
    m_chunkGenerator.reset(new ChunkGenerator(1337, *m_dynamicsWorld));
    m_mammut.reset(new Mammut(glm::vec3(0.0f, .7f, 4.5f), *m_dynamicsWorld));
    m_camera.reset(new GameCamera(*m_mammut));
    
    changeGravity(kDown);
    
    for (int i = 0; i < 6 ; i++)
        m_chunkList << m_chunkGenerator->nextChunk();
}

GameLogic::~GameLogic()
{
    
}

void GameLogic::update(float seconds)
{
    m_dynamicsWorld->stepSimulation(seconds, 30, 0.01f);
    m_camera->update();
    
    if (m_chunkList.first()->boundingBox().llf().z > m_camera->center().z)
    {
        m_chunkList.removeFirst();
        m_chunkList << m_chunkGenerator->nextChunk();
    }
}

void GameLogic::keyPressed(int key)
{
    switch (key)
    {
    case Qt::Key_W: changeGravity(kUp); break;
    case Qt::Key_A: changeGravity(kLeft); break;
    case Qt::Key_D: changeGravity(kRight); break;
    case Qt::LeftArrow: break;
    case Qt::RightArrow: break;
    }
}

void GameLogic::keyReleased(int key)
{
    switch (key)
    {
    case Qt::LeftArrow: break;
    case Qt::RightArrow: break;
    }
}

void GameLogic::initializeDynamicsWorld()
{
    m_broadphase.reset(new btDbvtBroadphase());
    m_collisionConfiguration.reset(new btDefaultCollisionConfiguration());
    m_dispatcher.reset(new btCollisionDispatcher(m_collisionConfiguration.data()));
    m_solver.reset(new btSequentialImpulseConstraintSolver());
    m_dynamicsWorld.reset(new btDiscreteDynamicsWorld(m_dispatcher.data(),
                                                      m_broadphase.data(),
                                                      m_solver.data(),
                                                      m_collisionConfiguration.data()));
}

void GameLogic::changeGravity(Gravity direction)
{
    m_activeGravity = static_cast<Gravity>((m_activeGravity + direction) % 4);
    
    const float gravityAcceleration = 9.81f;
    
    switch (m_activeGravity)
    {
        case kDown:  m_dynamicsWorld->setGravity(btVector3(0, -gravityAcceleration, 0)); break;
        case kLeft:  m_dynamicsWorld->setGravity(btVector3(-gravityAcceleration, 0, 0)); break;
        case kUp:    m_dynamicsWorld->setGravity(btVector3(0, gravityAcceleration, 0)); break;
        case kRight: m_dynamicsWorld->setGravity(btVector3(gravityAcceleration, 0, 0)); break;
    }
    
    m_mammut->changeGravity(m_activeGravity);
}

void GameLogic::simulationTickCallback(float timeStep)
{
    int manifoldsCount = m_dispatcher->getNumManifolds();
    for (int i = 0; i < manifoldsCount; i++)
    {
        btPersistentManifold * manifold = m_dispatcher->getManifoldByIndexInternal(i);
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
        mammut->update();
    }
}

const GameCamera & GameLogic::camera() const
{
    return *m_camera;
}

const Mammut & GameLogic::mammut() const
{
    return *m_mammut;
}

void GameLogic::forEachCuboid(const std::function<void (Cuboid &)> & lambda)
{
    for (QSharedPointer<CuboidChunk> & chunk : m_chunkList)
        for (Cuboid * cuboid : chunk->cuboids())
            lambda(*cuboid);
}




