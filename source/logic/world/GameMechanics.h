#pragma once

#include <functional>

#include <QList>

#include <logic/Mechanics.h>
#include <sound/Sound.h>

#include "PhysicsWorld.h"
#include "leveldesign/ChunkGenerator.h"
#include "Mammut.h"
#include "Cave.h"
#include "GameCamera.h"
#include "tets/BunchOfTets.h"


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
    const QList<Cuboid *> & cuboids() const;
    const QVector<const BunchOfTets *> bunches() const;
    int score() const;
    float lastZShift() const;

signals:
    void pause();
    void gameOver(int score);
    void waitForTetGenerator();

protected slots:
    void splitOneCuboid();
    
protected:
    void connectSignals();
    void updateSound();
    void zReset();
    void addAndRemoveCuboids();
    
    float normalizedMammutCaveDistance();
    bool mammutCollidesWithCave();
    
protected:
    static const float s_zResetDistance;
    static const float s_cuboidDeletionDistance;
    static const float s_cuboidCreationDistance;
    PhysicsWorld m_physicsWorld;
    
    ChunkGenerator m_chunkGenerator;
    Mammut m_mammut;
    Cave m_cave;
    GameCamera m_camera;
    QList<Cuboid *> m_cuboids;
    QVector<BunchOfTets *> m_bunches;
    
    float m_totalZShift;
    float m_lastZShift;
    bool m_gameOver;

    Sound m_backgroundLoop;

};
