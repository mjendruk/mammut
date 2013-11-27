#include "GameLogic.h"

#include <Qt>
#include <glm/gtx/transform.hpp>
#include <btBulletDynamicsCommon.h>

#include <QVector>

#include "Cuboid.h"
#include "GameCamera.h"
#include "Mammut.h"

GameLogic::GameLogic()
:   m_activeGravity(kDown)
{
    initializeDynamicsWorld();
    initializeTestlevel();
    
    m_mammut.reset(new Mammut(m_dynamicsWorld, glm::vec3(0.1), glm::vec3(0.0f, .7f, 4.5f)));
    m_camera.reset(new GameCamera(*m_mammut));
    
    changeGravity(kDown);
}  

GameLogic::~GameLogic()
{
    qDeleteAll(m_cuboids);
}

const QVector<Cuboid *> & GameLogic::cuboids() const
{
    return m_cuboids;
}

const Mammut & GameLogic::mammut() const
{
    return *m_mammut;
}

void GameLogic::update(int ms)
{
    m_dynamicsWorld->stepSimulation(ms / 1000.0f, 50, 0.01);
    
    m_mammut->update();
    m_camera->update();
}

void GameLogic::keyPressed(int key)
{
    switch (key)
    {
    case Qt::Key_W: changeGravity(kUp); break;
    case Qt::Key_A: changeGravity(kLeft); break;
    case Qt::Key_D: changeGravity(kRight); break;
    case Qt::LeftArrow: break;
    case Qt::RightArrow: break;
    }
}

void GameLogic::keyReleased(int key)
{
    switch (key)
    {
    case Qt::LeftArrow: break;
    case Qt::RightArrow: break;
    }
}

void GameLogic::initializeDynamicsWorld()
{
    btBroadphaseInterface* broadphase = new btDbvtBroadphase();
    btDefaultCollisionConfiguration * collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher * dispatcher = new btCollisionDispatcher(collisionConfiguration);
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();
    m_dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
}

void GameLogic::initializeTestlevel()
{
    m_cuboids << new Cuboid(glm::vec3(0.5f, 0.2f, 2.5f) * 15.f, glm::vec3(0.0f, .5f, 3.0f) * 5.f, *m_dynamicsWorld);
    m_cuboids << new Cuboid(glm::vec3(.3, .5f, 2.f) * 15.f, glm::vec3(0.5f, 0.0f, 0.0f) * 5.f, *m_dynamicsWorld);
    m_cuboids << new Cuboid(glm::vec3(.5f, .5f, 1.f) * 15.f, glm::vec3(-1.f, .50f, 0.0f) * 5.f, *m_dynamicsWorld);
    m_cuboids << new Cuboid(glm::vec3(0.3f, 0.2f, 1.f) * 15.f, glm::vec3(0.7f, -0.2f, -3.0f) * 5.f, *m_dynamicsWorld);
    m_cuboids << new Cuboid(glm::vec3(0.3f, 0.2f, 1.f) * 15.f, glm::vec3(0.7f, -0.2f, -5.0f) * 5.f, *m_dynamicsWorld);
    m_cuboids << new Cuboid(glm::vec3(0.3f, 0.2f, 1.f) * 15.f, glm::vec3(0.7f, -0.2f, -3.0f) * 5.f, *m_dynamicsWorld);
    m_cuboids << new Cuboid(glm::vec3(.5f, .5f, 1.f) * 15.f, glm::vec3(0.5f, 0.8f, -5.0f) * 5.f, *m_dynamicsWorld);
    m_cuboids << new Cuboid(glm::vec3(.3, .5f, 2.f) * 15.f, glm::vec3(-0.3f, 0.2f, -6.0f) * 5.f, *m_dynamicsWorld);
    m_cuboids << new Cuboid(glm::vec3(.5f, .5f, 1.f) * 15.f, glm::vec3(0.4f, -0.3f, -7.0f) * 5.f, *m_dynamicsWorld);
    m_cuboids << new Cuboid(glm::vec3(.4f, .5f, 1.5f) * 15.f, glm::vec3(-0.2f, 0.8f, -5.4f) * 5.f, *m_dynamicsWorld);
    m_cuboids << new Cuboid(glm::vec3(.1, .3f, 2.f) * 15.f, glm::vec3(-0.6f, -0.2f, -4.5f) * 5.f, *m_dynamicsWorld);
    m_cuboids << new Cuboid(glm::vec3(.5f, .5f, 1.7f) * 15.f, glm::vec3(-0.4f, -0.3f, -8.f) * 5.f, *m_dynamicsWorld);
    m_cuboids << new Cuboid(glm::vec3(.35, .5f, 2.f) * 15.f, glm::vec3(0.f, 0.0f, -10.0f) * 5.f, *m_dynamicsWorld);
    m_cuboids << new Cuboid(glm::vec3(.5f, .5f, 1.f) * 15.f, glm::vec3(0.f, 0.0f, -6.0f) * 5.f, *m_dynamicsWorld);
}


const GameCamera & GameLogic::camera() const
{
    return *m_camera;
}

void GameLogic::changeGravity(Gravity direction)
{
    m_activeGravity = static_cast<Gravity>((m_activeGravity + direction) % 4);
    
    const float gravityAcceleration = 9.81f;

    switch (m_activeGravity)
    {
    case kDown:  m_dynamicsWorld->setGravity(btVector3(0, gravityAcceleration, 0)); break;
    case kLeft:  m_dynamicsWorld->setGravity(btVector3(gravityAcceleration, 0, 0)); break;
    case kUp:    m_dynamicsWorld->setGravity(btVector3(0, -gravityAcceleration, 0)); break;
    case kRight: m_dynamicsWorld->setGravity(btVector3(-gravityAcceleration, 0, 0)); break;
    }

    m_mammut->setGravity(m_activeGravity);
}
