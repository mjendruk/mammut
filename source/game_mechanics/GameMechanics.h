#pragma once

#include <functional>

#include <QList>
#include <QSharedPointer>

#include <btBulletDynamicsCommon.h>

#include <Mechanics.h>
#include <game_world_rendering/GameWorldRenderer.h>

#include "ChunkGenerator.h"
#include "Mammut.h"
#include "Gravity.h"
#include "GameCamera.h"
#include "Gravity.h"


class GameMechanics : public Mechanics
{
public:
    GameMechanics();
    virtual ~GameMechanics();

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
    void registerTickCallbacks();
    
protected:
    btDbvtBroadphase m_broadphase;
    btDefaultCollisionConfiguration m_collisionConfiguration;
    btCollisionDispatcher m_dispatcher;
    btSequentialImpulseConstraintSolver m_solver;
    btDiscreteDynamicsWorld m_dynamicsWorld;
    
    ChunkGenerator m_chunkGenerator;
    Mammut m_mammut;
    GameCamera m_camera;
    Gravity m_gravity;
    QList<QSharedPointer<CuboidChunk>> m_chunkList;
    
    GameWorldRenderer m_renderer;

};
