#include "GameLogic.h"

#include <glm/gtx/transform.hpp>

#include <QDebug>
#include <QThread>
#include <QVector>

#include <btBulletDynamicsCommon.h>

#include "Cuboid.h"
#include "Mammoth.h"
#include "RenderCamera.h"

GameLogic::GameLogic()
:   m_activeGravity(gravity::down)
{
    btBroadphaseInterface* broadphase = new btDbvtBroadphase();
    btDefaultCollisionConfiguration * collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher * dispatcher = new btCollisionDispatcher(collisionConfiguration);
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();
    m_dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
    m_dynamicsWorld->setGravity(btVector3(0, -9.81, 0));

    m_mammoth.reset(new Mammoth(m_dynamicsWorld, glm::vec3(0.1), glm::vec3(0.0f, .7f, 4.5f)));
    m_mammoth->setGravity(gravity::down);

    m_camera.reset(new GameCamera(*m_mammoth));

    initializeTestlevel();
}  

GameLogic::~GameLogic()
{
    qDeleteAll(m_cuboids);
}

const QVector<Cuboid *> & GameLogic::cuboids() const
{
    return m_cuboids;
}

const Mammoth & GameLogic::mammoth() const
{
    return *m_mammoth;
}

void GameLogic::update(int ms)
{
    QThread::msleep(2);

    m_dynamicsWorld->stepSimulation(ms / 1000.0f, 50, 0.01);

    for (Cuboid * cuboid : m_cuboids)
    {
        cuboid->updatePhysics();
    }
    
    m_mammoth->updatePhysics();

    m_camera->update();
}

void GameLogic::keyPressed(int key)
{
    switch (key)
    {
    case Qt::Key_W: changeGravity(2); break;
    case Qt::Key_A: changeGravity(1); break;
    case Qt::Key_D: changeGravity(-1); break;
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

void GameLogic::initializeTestlevel()
{
    //m_cuboids << new Cuboid(m_dynamicsWorld, glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.0f, 0.0f));

    m_cuboids << new Cuboid(m_dynamicsWorld, glm::vec3(0.5f, 0.2f, 2.5f) * 15.f, glm::vec3(0.0f, .5f, 3.0f) * 5.f);

    m_cuboids << new Cuboid(m_dynamicsWorld, glm::vec3(.3, .5f, 2.f) * 15.f, glm::vec3(0.5f, 0.0f, 0.0f) * 5.f);
    m_cuboids << new Cuboid(m_dynamicsWorld, glm::vec3(.5f, .5f, 1.f) * 15.f, glm::vec3(-1.f, .50f, 0.0f) * 5.f);
    m_cuboids << new Cuboid(m_dynamicsWorld, glm::vec3(0.3f, 0.2f, 1.f) * 15.f, glm::vec3(0.7f, -0.2f, -3.0f) * 5.f);
    m_cuboids << new Cuboid(m_dynamicsWorld, glm::vec3(0.3f, 0.2f, 1.f) * 15.f, glm::vec3(0.7f, -0.2f, -5.0f) * 5.f);
    m_cuboids << new Cuboid(m_dynamicsWorld, glm::vec3(0.3f, 0.2f, 1.f) * 15.f, glm::vec3(0.7f, -0.2f, -3.0f) * 5.f);
    m_cuboids << new Cuboid(m_dynamicsWorld, glm::vec3(.5f, .5f, 1.f) * 15.f, glm::vec3(0.5f, 0.8f, -5.0f) * 5.f);
    m_cuboids << new Cuboid(m_dynamicsWorld, glm::vec3(.3, .5f, 2.f) * 15.f, glm::vec3(-0.3f, 0.2f, -6.0f) * 5.f);
    m_cuboids << new Cuboid(m_dynamicsWorld, glm::vec3(.5f, .5f, 1.f) * 15.f, glm::vec3(0.4f, -0.3f, -7.0f) * 5.f);
    m_cuboids << new Cuboid(m_dynamicsWorld, glm::vec3(.4f, .5f, 1.5f) * 15.f, glm::vec3(-0.2f, 0.8f, -5.4f) * 5.f);
    m_cuboids << new Cuboid(m_dynamicsWorld, glm::vec3(.1, .3f, 2.f) * 15.f, glm::vec3(-0.6f, -0.2f, -4.5f) * 5.f);
    m_cuboids << new Cuboid(m_dynamicsWorld, glm::vec3(.5f, .5f, 1.7f) * 15.f, glm::vec3(-0.4f, -0.3f, -8.f) * 5.f);
    m_cuboids << new Cuboid(m_dynamicsWorld, glm::vec3(.35, .5f, 2.f) * 15.f, glm::vec3(0.f, 0.0f, -10.0f) * 5.f);
    m_cuboids << new Cuboid(m_dynamicsWorld, glm::vec3(.5f, .5f, 1.f) * 15.f, glm::vec3(0.f, 0.0f, -6.0f) * 5.f);
}


const GameCamera & GameLogic::camera() const
{
    return *m_camera;
}

void GameLogic::changeGravity(int delta)
{
    m_activeGravity = (m_activeGravity + 4 + delta) % 4;


    switch (m_activeGravity)
    {
    case gravity::up:   m_dynamicsWorld->setGravity(btVector3(0, 9.81, 0)); 
                        break;
    case gravity::down: m_dynamicsWorld->setGravity(btVector3(0, -9.81, 0)); break;
    case gravity::right:m_dynamicsWorld->setGravity(btVector3(9.81, 0, 0)); break;
    case gravity::left: m_dynamicsWorld->setGravity(btVector3(-9.81, 0, 0)); break;
    }

    m_mammoth->setGravity(m_activeGravity);
}
