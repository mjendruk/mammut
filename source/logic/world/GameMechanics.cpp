#include "GameMechanics.h"

#include <chrono>

#include <QKeyEvent>
#include <QTimer>

#include <glowutils/AxisAlignedBoundingBox.h>

#include <sound/SoundManager.h>
#include "tets/Tet.h"
#include "tets/TetGenerator.h"
#include "Cuboid.h"
#include <PerfCounter.h>

const float GameMechanics::s_zResetDistance = 800.f;
const float GameMechanics::s_cuboidDeletionDistance = 100.0f;
const float GameMechanics::s_cuboidCreationDistance = 700.0f;

GameMechanics::GameMechanics()
:   m_chunkGenerator(std::chrono::system_clock::now().time_since_epoch().count())
,   m_mammut(glm::vec3(0.f, 0.05f, 0.0f))
,   m_totalZShift(0.0f)
,   m_lastZShift(0.0f)
,   m_gameOver(false)
,   m_backgroundLoop(Sound::kLoop, true)
{
    connectSignals();

    QTimer::singleShot(1500, this, SLOT(splitOneCuboid()));

    addAndRemoveCuboids();
    
    m_physicsWorld.addObject(m_mammut.physics());
    m_physicsWorld.setMammutPhysics(m_mammut.physics());
    m_physicsWorld.changeGravity(PhysicsWorld::kGravityDown);
}

GameMechanics::~GameMechanics()
{
    waitForTetGenerator();
    for (Cuboid * cuboid : m_cuboids)
        m_physicsWorld.removeObject(cuboid);

    qDeleteAll(m_cuboids);
    
    m_physicsWorld.removeObject(m_mammut.physics());
    m_backgroundLoop.stop();
}

void GameMechanics::splitOneCuboid()
{
    Cuboid * cuboid = m_cuboids.takeAt(2);
    m_physicsWorld.removeObject(cuboid);
    m_bunches << cuboid->splitIntoTets();
}

void GameMechanics::update(float seconds)
{
    if (m_gameOver) {
        emit gameOver(score());
        return;
    }
    PerfCounter::begin("game");

    m_lastZShift = 0.0f;
    
    m_backgroundLoop.setPaused(false);
    m_physicsWorld.stepSimulation(seconds);
    
    addAndRemoveCuboids();

    for (BunchOfTets * bunch : m_bunches)
        bunch->update(seconds, m_physicsWorld);

    if (m_mammut.position().z < -s_zResetDistance)
        zReset();

    updateSound();
    PerfCounter::end("game");
}

void GameMechanics::tickUpdate(float seconds)
{
    m_mammut.update();
    
    if (mammutCollidesWithCave())
        m_mammut.caveCollisionEvent();
    
    m_camera.update(m_mammut.position(), m_mammut.velocity(), seconds, normalizedMammutCaveDistance());
}

float GameMechanics::normalizedMammutCaveDistance()
{
    return glm::length(m_mammut.position().xy()) / (Cave::s_caveRadius - Cave::s_caveDeathDistance);
}

bool GameMechanics::mammutCollidesWithCave()
{
    return glm::length(m_mammut.position().xy()) >= (Cave::s_caveRadius - Cave::s_caveDeathDistance);
}

void GameMechanics::updateSound()
{
    glm::vec3 forward =  glm::normalize(m_camera.center() - m_camera.eye());
    glm::vec3 velocity = glm::vec3(0.0, 0.0, m_mammut.velocity().z);
    SoundManager::instance().setListenerAttributes(m_mammut.position(), forward, m_camera.up(), velocity);
}

void GameMechanics::zReset()
{
    float zShift = -m_mammut.position().z;
    m_mammut.addZShift(zShift);
    m_cave.addZShift(zShift);
    m_chunkGenerator.addZShift(zShift);
    for (Cuboid * cuboid : m_cuboids)
        cuboid->addZShift(zShift);

    m_camera.update(m_mammut.position(), m_mammut.velocity(), 0.0f, normalizedMammutCaveDistance());

    m_totalZShift += zShift;
    m_lastZShift = zShift;

    for (BunchOfTets * bunch : m_bunches)
        bunch->addZShift(zShift);

}

void GameMechanics::addAndRemoveCuboids()
{
    float createCuboidsUpTo = m_mammut.position().z - s_cuboidCreationDistance;
    while (m_cuboids.isEmpty() || m_cuboids.last()->boundingBox().llf().z > createCuboidsUpTo) {
        QList<Cuboid *> newCuboids = m_chunkGenerator.nextChunk();
        for (Cuboid * cuboid : newCuboids)
            m_physicsWorld.addObject(cuboid);
        m_cuboids << newCuboids;
    }

    float deleteCuboidsUpTo = m_mammut.position().z + s_cuboidDeletionDistance;
    while (m_cuboids.first()->boundingBox().llf().z > deleteCuboidsUpTo) {
        Cuboid * cuboidToRemove = m_cuboids.takeFirst();
        m_physicsWorld.removeObject(cuboidToRemove);
        delete cuboidToRemove;
    }
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
    case Qt::Key_Left:
        m_mammut.applyBoost(Mammut::BoostDirection::kLeft);
        break;
    case Qt::Key_Up:
        m_mammut.applyBoost(Mammut::BoostDirection::kUp);
        break;
    case Qt::Key_Right:
        m_mammut.applyBoost(Mammut::BoostDirection::kRight);
        break;
    case Qt::Key_Down:
        m_mammut.applyBoost(Mammut::BoostDirection::kDown);
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

const Cave & GameMechanics::cave() const
{
    return m_cave;
}

const QList<Cuboid *> & GameMechanics::cuboids() const
{
    return m_cuboids;
}


const QVector<const BunchOfTets *> GameMechanics::bunches() const
{
    //constify all the pointers
    QVector<const BunchOfTets *> temp;
    for (BunchOfTets * bunch : m_bunches)
        temp << bunch;
    return temp;
}

int GameMechanics::score() const
{
    return int(m_totalZShift + -m_mammut.position().z);
}

float GameMechanics::lastZShift() const
{
    return m_lastZShift;
}

void GameMechanics::connectSignals()
{
    connect(&m_physicsWorld, &PhysicsWorld::simulationTick, this, &GameMechanics::tickUpdate);
    connect(&m_physicsWorld, &PhysicsWorld::gravityChanged, &m_camera, &GameCamera::gravityChangeEvent);
    connect(&m_physicsWorld, &PhysicsWorld::gravityChanged, &m_mammut, &Mammut::gravityChangeEvent);
    connect(this, &GameMechanics::pause, &m_camera, &GameCamera::pauseEvent);
    connect(this, &GameMechanics::waitForTetGenerator, &TetGenerator::instance(), &TetGenerator::dummySlot, Qt::BlockingQueuedConnection);
    
    connect(&m_mammut, &Mammut::crashed, [this]() {
        m_gameOver = true;
    });
}
