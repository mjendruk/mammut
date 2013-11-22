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
{
    btBroadphaseInterface* broadphase = new btDbvtBroadphase();
    btDefaultCollisionConfiguration * collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher * dispatcher = new btCollisionDispatcher(collisionConfiguration);
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();
    m_dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
    m_dynamicsWorld->setGravity(btVector3(0, -9.81, 0));

    m_mammoth.reset(new Mammoth(m_dynamicsWorld, glm::vec3(0.1), glm::vec3(-0.05f, .7f, 2.f)));

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

    m_dynamicsWorld->stepSimulation(1 / 200.f);

    for (Cuboid * cuboid : m_cuboids)
    {
        cuboid->updatePhysics();
    }
    
    m_mammoth->updatePhysics();

    m_camera->update();
}

void GameLogic::keyPressed(int key)
{
    /*switch (key)
    {
    case Qt::Key_W: //m_fallRigidBody->setLinearVelocity(m_fallRigidBody->getLinearVelocity() + btVector3(0, 0, 5));
                    m_camera.modifyPanVector(glm::vec3(0.0f, 0.0f, -0.01f));
                    break;
    case Qt::Key_A: m_camera.modifyPanVector(glm::vec3(-0.01f, 0.0f, 0.0f));
                    break;
    case Qt::Key_S: m_camera.modifyPanVector(glm::vec3(0.0f, 0.0f, 0.01f));
                    break;
    case Qt::Key_D: m_camera.modifyPanVector(glm::vec3(0.01f, 0.0f, 0.0f));
                    break;
    case Qt::Key_Z: m_camera.modifyPanVector(glm::vec3(0.0f, 0.01f, 0.0f));
                    break;
    case Qt::Key_H: m_camera.modifyPanVector(glm::vec3(0.0f, -0.01f, 0.0f));
                    break;
    }*/
}

void GameLogic::keyReleased(int key)
{
    /*switch (key)
    {
    case Qt::Key_W: //m_fallRigidBody->setLinearVelocity(m_fallRigidBody->getLinearVelocity() - btVector3(0, 0, 5));
                    m_camera.modifyPanVector(glm::vec3(0.0f, 0.0f, 0.01f));
                    break;
    case Qt::Key_A: m_camera.modifyPanVector(glm::vec3(0.01f, 0.0f, 0.0f)); 
                    break;
    case Qt::Key_S: m_camera.modifyPanVector(glm::vec3(0.0f, 0.0f, -0.01f)); 
                    break;
    case Qt::Key_D: m_camera.modifyPanVector(glm::vec3(-0.01f, 0.0f, 0.0f)); 
                    break;
    case Qt::Key_Z: m_camera.modifyPanVector(glm::vec3(0.0f , -0.01f, 0.0f));
                    break;
    case Qt::Key_H: m_camera.modifyPanVector(glm::vec3(0.0f, 0.01f, 0.0f));
                    break;
    }*/
}

void GameLogic::initializeTestlevel()
{
    //m_cuboids << new Cuboid(m_dynamicsWorld, glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.0f, 0.0f));

    m_cuboids << new Cuboid(m_dynamicsWorld, glm::vec3(0.5f, 0.2f, 2.5f), glm::vec3(-0.25f, .5f, 3.0f));

    m_cuboids << new Cuboid(m_dynamicsWorld, glm::vec3(.3, .5f, 2.f), glm::vec3(0.5f, 0.0f, 0.0f));
    m_cuboids << new Cuboid(m_dynamicsWorld, glm::vec3(.5f, .5f, 1.f), glm::vec3(-1.f, .50f, 0.0f));
    m_cuboids << new Cuboid(m_dynamicsWorld, glm::vec3(0.3f, 0.2f, 1.f), glm::vec3(0.7f, -0.2f, -3.0f));
    m_cuboids << new Cuboid(m_dynamicsWorld, glm::vec3(0.3f, 0.2f, 1.f), glm::vec3(0.7f, -0.2f, -5.0f));
    m_cuboids << new Cuboid(m_dynamicsWorld, glm::vec3(0.3f, 0.2f, 1.f), glm::vec3(0.7f, -0.2f, -3.0f));
    m_cuboids << new Cuboid(m_dynamicsWorld, glm::vec3(.5f, .5f, 1.f), glm::vec3(0.5f, 0.8f, -5.0f));
    m_cuboids << new Cuboid(m_dynamicsWorld, glm::vec3(.3, .5f, 2.f), glm::vec3(-0.3f, 0.2f, -6.0f));
    m_cuboids << new Cuboid(m_dynamicsWorld, glm::vec3(.5f, .5f, 1.f), glm::vec3(0.4f, -0.3f, -7.0f));
    m_cuboids << new Cuboid(m_dynamicsWorld, glm::vec3(.4f, .5f, 1.5f), glm::vec3(-0.2f, 0.8f, -5.4f));
    m_cuboids << new Cuboid(m_dynamicsWorld, glm::vec3(.1, .3f, 2.f), glm::vec3(-0.6f, -0.2f, -4.5f));
    m_cuboids << new Cuboid(m_dynamicsWorld, glm::vec3(.5f, .5f, 1.7f), glm::vec3(-0.4f, -0.3f, -8.f));
    m_cuboids << new Cuboid(m_dynamicsWorld, glm::vec3(.35, .5f, 2.f), glm::vec3(0.f, 0.0f, -10.0f));
    m_cuboids << new Cuboid(m_dynamicsWorld, glm::vec3(.5f, .5f, 1.f), glm::vec3(0.f, 0.0f, -6.0f));
}


const GameCamera & GameLogic::camera() const
{
    return *m_camera;
}