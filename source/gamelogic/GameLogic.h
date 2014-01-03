#pragma once

#include <functional>

#include <QList>
#include <QSharedPointer>
#include <QScopedPointer>

#include "Cuboid.h"
#include "ChunkGenerator.h"
#include "Gravity.h"

class btDiscreteDynamicsWorld;
class btSequentialImpulseConstraintSolver;
class btCollisionDispatcher;
class btDefaultCollisionConfiguration;
class btBroadphaseInterface;

class GameCamera;
class Mammut;

class GameLogic
{
public:
    GameLogic();
    ~GameLogic();

    void update(float seconds);
    void keyPressed(int key);
    void keyReleased(int key);
    
    const GameCamera & camera() const;
    const Mammut & mammut() const;
    
    void forEachCuboid(const std::function<void(Cuboid &)> & lambda);
    
    void preTickCallback(float timeStep);
    void postTickCallback(float timeStep);
    
protected:
    void initializeDynamicsWorld();
    
protected:
    QList<QSharedPointer<CuboidChunk>> m_chunkList;
    
    QScopedPointer<ChunkGenerator> m_chunkGenerator;
    QScopedPointer<Mammut> m_mammut;
    QScopedPointer<GameCamera> m_camera;
    QScopedPointer<Gravity> m_gravity;

    QScopedPointer<btDiscreteDynamicsWorld> m_dynamicsWorld;
    QScopedPointer<btSequentialImpulseConstraintSolver> m_solver;
    QScopedPointer<btCollisionDispatcher> m_dispatcher;
    QScopedPointer<btDefaultCollisionConfiguration> m_collisionConfiguration;
    QScopedPointer<btBroadphaseInterface> m_broadphase;
};
