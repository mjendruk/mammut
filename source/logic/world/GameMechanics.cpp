#include "GameMechanics.h"

#include <QKeyEvent>

#include <sound/SoundManager.h>

GameMechanics::GameMechanics()
:   m_chunkGenerator(1337)
,   m_mammut(glm::vec3(-2.2f, 7.6f, 15.0f))
,   m_gameOver(false)
,   m_backgroundLoop(Sound::kLoop, true)
{
    connectSignals();
    
    for (int i = 0; i < 7; ++i)
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
    m_backgroundLoop.stop();
}

void GameMechanics::update(float seconds)
{
    if (m_gameOver) {
        emit gameOver(std::max(0, static_cast<int>(-m_mammut.position().z)));
        return;
    }
    
    m_backgroundLoop.setPaused(false);
    m_physicsWorld.stepSimulation(seconds);
    
    m_camera.update(m_mammut.position(), m_mammut.velocity(), seconds);
    
    if (m_chunkList.at(1)->boundingBox().llf().z > m_camera.center().z) {
        for (auto cuboid : m_chunkList.first()->cuboids())
            m_physicsWorld.removeObject(cuboid);
        m_chunkList.removeFirst();

        m_chunkList << m_chunkGenerator.nextChunk();
        for (auto cuboid : m_chunkList.last()->cuboids()) {
            m_physicsWorld.addObject(cuboid);
        }
    }

    updateSound();
}

void GameMechanics::updateSound()
{
    glm::vec3 forward =  glm::normalize(m_camera.center() - m_camera.eye());
    glm::vec3 velocity = glm::vec3(0.0, 0.0, m_mammut.velocity().z);
    SoundManager::instance().setListenerAttributes(m_mammut.position(), forward, m_camera.up(), velocity);
}

void GameMechanics::keyPressed(QKeyEvent * event)
{
    switch (event->key())
    {
    case Qt::Key_Escape:
        {
        Sound sound(Sound::kButtonClick);
        m_backgroundLoop.setPaused(true);
        emit pause();
        break;
        }
    case Qt::Key_W:
        m_physicsWorld.changeGravity(PhysicsWorld::kGravityUp);
        break;
    case Qt::Key_A:
        m_physicsWorld.changeGravity(PhysicsWorld::kGravityLeft);
        break;
    case Qt::Key_D:
        m_physicsWorld.changeGravity(PhysicsWorld::kGravityRight);
        break;
    }
}

void GameMechanics::keyReleased(QKeyEvent * event)
{
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
    for (auto chunk : m_chunkList)
        for (Cuboid * cuboid : chunk->cuboids())
            lambda(cuboid);
}

void GameMechanics::forEachCuboid(const std::function<void(const Cuboid *)> & lambda) const
{
    for (auto chunk : m_chunkList)
        for (Cuboid * cuboid : chunk->cuboids())
            lambda(cuboid);
}

void GameMechanics::connectSignals()
{
    connect(&m_physicsWorld, &PhysicsWorld::simulationTick, &m_mammut, &Mammut::update);
    connect(&m_physicsWorld, &PhysicsWorld::gravityChanged, &m_camera, &GameCamera::gravityChangeEvent);
    connect(&m_physicsWorld, &PhysicsWorld::gravityChanged, &m_mammut, &Mammut::gravityChangeEvent);
    
    connect(&m_mammut, &Mammut::crashed, [this]() {
        m_gameOver = true;
    });
}
