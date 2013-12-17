#include "GameLogic.h"

#include <cassert>

#include <QDebug>
#include <QVector>

#include <glm/gtx/transform.hpp>
#include <btBulletDynamicsCommon.h>

#include "Cuboid.h"
#include "GameCamera.h"
#include "Mammut.h"

namespace Callbacks
{
    
void preTickCallback(btDynamicsWorld * world, btScalar timeStep)
{
    GameLogic * gameLogic = static_cast<GameLogic *>(world->getWorldUserInfo());
    gameLogic->preTickCallback(timeStep);
}
    
void postTickCallback(btDynamicsWorld * world, btScalar timeStep)
{
    GameLogic * gameLogic = static_cast<GameLogic *>(world->getWorldUserInfo());
    gameLogic->postTickCallback(timeStep);
}

} // namespace

GameLogic::GameLogic()
{
    initializeDynamicsWorld();
    
    m_chunkGenerator.reset(new ChunkGenerator(1337, *m_dynamicsWorld));

    m_mammut.reset(new Mammut(glm::vec3(0.0f, .7f, 4.5f), *m_dynamicsWorld));
    m_gravity.reset(new Gravity(*m_dynamicsWorld));
    m_camera.reset(new GameCamera(*m_mammut));
    
    m_gravity->rotate(Gravity::kDown);
                    
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
    case Qt::Key_W:
        m_gravity->rotate(Gravity::kUp);
        m_mammut->rotate(m_gravity->rotation());
        break;
    case Qt::Key_A:
        m_gravity->rotate(Gravity::kLeft);
        m_mammut->rotate(m_gravity->rotation());
        break;
    case Qt::Key_D:
        m_gravity->rotate(Gravity::kRight);
        m_mammut->rotate(m_gravity->rotation());
        break;
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
    
    m_dynamicsWorld->setInternalTickCallback(Callbacks::preTickCallback,
                                             static_cast<void *>(this), true);
    m_dynamicsWorld->setInternalTickCallback(Callbacks::postTickCallback,
                                             static_cast<void *>(this), false);
}

void GameLogic::preTickCallback(float timeStep)
{
    m_mammut->applyForces();
    m_mammut->setIsOnObject(false);
}

void GameLogic::postTickCallback(float timeStep)
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
        
        mammut->collidesWith(*cuboid, m_gravity->inverseRotation());
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




