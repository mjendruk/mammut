#pragma once

#include <functional>

#include <QList>
#include <QSharedPointer>
#include <QScopedPointer>

#include "Commons.h"
#include "Cuboid.h"
#include "ChunkGenerator.h"

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

    void update(int ms);
    void keyPressed(int key);
    void keyReleased(int key);
    
    const GameCamera & camera() const;
    const Mammut & mammut() const;
    
    void forEachCuboid(const std::function<void(Cuboid &)> & lambda);

protected:
    void initializeDynamicsWorld();
    
    void changeGravity(Gravity direction);
    
protected:
    Gravity m_activeGravity;
    
    QList<QSharedPointer<CuboidChunk>> m_chunkList;
    
    QScopedPointer<ChunkGenerator> m_chunkGenerator;
    QScopedPointer<Mammut> m_mammut;
    QScopedPointer<GameCamera> m_camera;

    QScopedPointer<btDiscreteDynamicsWorld> m_dynamicsWorld;
    QScopedPointer<btSequentialImpulseConstraintSolver> m_solver;
    QScopedPointer<btCollisionDispatcher> m_dispatcher;
    QScopedPointer<btDefaultCollisionConfiguration> m_collisionConfiguration;
    QScopedPointer<btBroadphaseInterface> m_broadphase;
};
