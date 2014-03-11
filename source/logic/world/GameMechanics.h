#pragma once

#include <functional>

#include <QList>
#include <QSharedPointer>

#include <logic/Mechanics.h>
#include <sound/Sound.h>

#include "PhysicsWorld.h"
#include "ChunkGenerator.h"
#include "Mammut.h"
#include "Cave.h"
#include "GameCamera.h"


class GameMechanics : public Mechanics
{
    Q_OBJECT

public:
    GameMechanics();
    virtual ~GameMechanics();

    void update(float seconds) override;
    void tickUpdate(float seconds);

    void keyPressed(QKeyEvent * event) override;
    void keyReleased(QKeyEvent * event) override;
    
    const GameCamera & camera() const;
    const Mammut & mammut() const;
    const Cave & cave() const;
    int score() const;
    
    void forEachCuboid(const std::function<void(Cuboid *)> & lambda);
    void forEachCuboid(const std::function<void(const Cuboid *)> & lambda) const;

signals:
    void pause();
    void gameOver(int score);
    
protected:
    void connectSignals();
    void updateSound();
    void zReset();
    
    float normalizedMammutCaveDistance();
    bool mammutCollidesWithCave();
    
protected:
    PhysicsWorld m_physicsWorld;
    
    ChunkGenerator m_chunkGenerator;
    Mammut m_mammut;
    Cave m_cave;
    GameCamera m_camera;
    QList<QSharedPointer<CuboidChunk>> m_chunkList;
    
    float m_zShift;
    bool m_gameOver;

    Sound m_backgroundLoop;

};
