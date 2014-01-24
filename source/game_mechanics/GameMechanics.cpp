#include "GameMechanics.h"

#include <QKeyEvent>

GameMechanics::GameMechanics()
:   m_chunkGenerator(0)
,   m_mammut(glm::vec3(-2.2f, 7.6f, 15.0f))
,   m_camera(m_mammut)
{
    connectSignals();
    
    for (int i = 0; i < 10 ; i++)
    {
        m_chunkList << m_chunkGenerator.nextChunk();
        for (auto cuboid : m_chunkList.last()->cuboids())
            m_physicsWorld.addObject(cuboid);
    }
    
    m_physicsWorld.addObject(m_mammut.physics());
    m_physicsWorld.changeGravity(PhysicsWorld::kGravityDown);
}

GameMechanics::~GameMechanics()
{
    forEachCuboid([this](Cuboid * cuboid) {
        m_physicsWorld.removeObject(cuboid);
    });
    
    m_physicsWorld.removeObject(m_mammut.physics());
}

void GameMechanics::update(float seconds)
{
    m_physicsWorld.stepSimulation(seconds);
    m_camera.update(seconds);
    
    if (m_chunkList.at(1)->boundingBox().llf().z > m_camera.center().z)
    {
        for (auto cuboid : m_chunkList.first()->cuboids())
            m_physicsWorld.removeObject(cuboid);
        m_chunkList.removeFirst();

        m_chunkList << m_chunkGenerator.nextChunk();
        for (auto cuboid : m_chunkList.last()->cuboids()) {
            m_physicsWorld.addObject(cuboid);
        }
    }
}

void GameMechanics::keyPressed(QKeyEvent * event)
{
    switch (event->key())
    {
    case Qt::Key_Escape:
        emit pause();
        break;
    case Qt::Key_W:
        m_physicsWorld.changeGravity(PhysicsWorld::kGravityUp);
        break;
    case Qt::Key_A:
        m_physicsWorld.changeGravity(PhysicsWorld::kGravityLeft);
        break;
    case Qt::Key_D:
        m_physicsWorld.changeGravity(PhysicsWorld::kGravityRight);
        break;
    case Qt::Key_Left:
//        m_mammut.steerLeft();
        break;
    case Qt::Key_Right:
//        m_mammut.steerRight();
        break;
    }
}

void GameMechanics::keyReleased(QKeyEvent * event)
{
    switch (event->key())
    {
    case Qt::Key_Left:
    case Qt::Key_Right:
//        m_mammut.doNotSteer();
        break;
    }
}

const GameCamera & GameMechanics::camera() const
{
    return m_camera;
}

const Mammut & GameMechanics::mammut() const
{
    return m_mammut;
}

void GameMechanics::forEachCuboid(const std::function<void (Cuboid *)> & lambda)
{
    for (QSharedPointer<CuboidChunk> & chunk : m_chunkList)
        for (Cuboid * cuboid : chunk->cuboids())
            lambda(cuboid);
}

void GameMechanics::connectSignals()
{
    connect(&m_physicsWorld, &PhysicsWorld::simulationTick, &m_mammut, &Mammut::update);
    connect(&m_physicsWorld, &PhysicsWorld::gravityChanged, &m_mammut, &Mammut::gravityChangeEvent);
    connect(&m_mammut, &Mammut::crashed, this, &GameMechanics::gameOver);
}




