#pragma once

#include <functional>

#include <QList>
#include <QSharedPointer>
#include <QScopedPointer>

#include <Mechanics.h>

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
class GameWorldRenderer;

class GameMechanics : public Mechanics
{
public:
    GameMechanics();
    ~GameMechanics();

    void update(float seconds) override;
    Renderer * renderer() override;

    void keyPressed(QKeyEvent * event) override;
    void keyReleased(QKeyEvent * event) override;
    
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
    
    QScopedPointer<GameWorldRenderer> m_renderer;

    QScopedPointer<btDiscreteDynamicsWorld> m_dynamicsWorld;
    QScopedPointer<btSequentialImpulseConstraintSolver> m_solver;
    QScopedPointer<btCollisionDispatcher> m_dispatcher;
    QScopedPointer<btDefaultCollisionConfiguration> m_collisionConfiguration;
    QScopedPointer<btBroadphaseInterface> m_broadphase;

};
