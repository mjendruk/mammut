#pragma once

#include <functional>

#include <QList>
#include <QSharedPointer>

#include <logic/Mechanics.h>

#include "PhysicsWorld.h"
#include "ChunkGenerator.h"
#include "Mammut.h"
#include "GameCamera.h"


class GameMechanics : public Mechanics
{
    Q_OBJECT

public:
    GameMechanics();
    virtual ~GameMechanics();

    void update(float seconds) override;
    void updateSound();

    void keyPressed(QKeyEvent * event) override;
    void keyReleased(QKeyEvent * event) override;
    
    const GameCamera & camera() const;
    const Mammut & mammut() const;
    
    void forEachCuboid(const std::function<void(Cuboid *)> & lambda);
    void forEachCuboid(const std::function<void(const Cuboid *)> & lambda) const;

signals:
    void pause();
    void gameOver();
    
protected:
    void connectSignals();
    
protected:
    PhysicsWorld m_physicsWorld;
    
    ChunkGenerator m_chunkGenerator;
    Mammut m_mammut;
    GameCamera m_camera;
    QList<QSharedPointer<CuboidChunk>> m_chunkList;

};
