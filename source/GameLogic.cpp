#include "GameLogic.h"

#include <glm/gtx/transform.hpp>

#include <QDebug>
#include <QThread>
#include <QVector>

#include <btBulletDynamicsCommon.h>

#include "Cuboid.h"
#include "RenderCamera.h"

GameLogic::GameLogic()
:   m_camera(glm::vec3(0.f, 1.2f, 2.4f), glm::vec3(0.f, 0.0f, 0.0f), glm::vec3(0.f, 1.0f, 0.0f))
{
    initialize();
}  

GameLogic::~GameLogic()
{
    qDeleteAll(m_cuboids);
}

const QVector<Cuboid *> & GameLogic::cuboids() const
{
    return m_cuboids;
}

void GameLogic::initialize()
{

    btBroadphaseInterface* broadphase = new btDbvtBroadphase();
    btDefaultCollisionConfiguration * collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher * dispatcher = new btCollisionDispatcher(collisionConfiguration);
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();
    m_dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
    m_dynamicsWorld->setGravity(btVector3(0, -9.81, 0));

    glm::mat4 mat = glm::translate(0.f, 10.0f, 0.0f);

    initializeTestlevel();
}

void GameLogic::update(int ms)
{
    QThread::msleep(2);

    m_dynamicsWorld->stepSimulation(1 / 200.f);

    for (Cuboid * cuboid : m_cuboids)
    {
        cuboid->updatePhysics();
    }
     
    m_camera.pan();
}

void GameLogic::keyPressed(int key)
{
    switch (key)
    {
    case Qt::Key_W: //m_fallRigidBody->setLinearVelocity(m_fallRigidBody->getLinearVelocity() + btVector3(0, 0, 5));
                    m_camera.modifyPanVector(glm::vec3(0.0f, 0.0f, -0.01f));
                    qDebug() << "Key  w pressed";
                    break;
    case Qt::Key_A: m_camera.modifyPanVector(glm::vec3(-0.01f, 0.0f, 0.0f));
                    qDebug() << "Key  a pressed";
                    break;
    case Qt::Key_S: m_camera.modifyPanVector(glm::vec3(0.0f, 0.0f, 0.01f));
                    qDebug() << "Key  s pressed";
                    break;
    case Qt::Key_D: m_camera.modifyPanVector(glm::vec3(0.01f, 0.0f, 0.0f));
                    qDebug() << "Key  d pressed";
                    break;
    case Qt::Key_Z: m_camera.modifyPanVector(glm::vec3(0.0f, 0.01f, 0.0f));
                    qDebug() << "Key  z pressed";
                    break;
    case Qt::Key_H: m_camera.modifyPanVector(glm::vec3(0.0f, -0.01f, 0.0f));
                    qDebug() << "Key  h pressed";
                    break;
    }
}

void GameLogic::keyReleased(int key)
{
    switch (key)
    {
    case Qt::Key_W: //m_fallRigidBody->setLinearVelocity(m_fallRigidBody->getLinearVelocity() - btVector3(0, 0, 5));
                    m_camera.modifyPanVector(glm::vec3(0.0f, 0.0f, 0.01f));
                    qDebug() << "Key  w released";
                    break;
    case Qt::Key_A: m_camera.modifyPanVector(glm::vec3(0.01f, 0.0f, 0.0f)); 
                    qDebug() << "Key  a released";
                    break;
    case Qt::Key_S: m_camera.modifyPanVector(glm::vec3(0.0f, 0.0f, -0.01f)); 
                    qDebug() << "Key  s released";
                    break;
    case Qt::Key_D: m_camera.modifyPanVector(glm::vec3(-0.01f, 0.0f, 0.0f)); 
                    qDebug() << "Key  d released";
                    break;
    case Qt::Key_Z: m_camera.modifyPanVector(glm::vec3(0.0f , -0.01f, 0.0f));
        qDebug() << "Key  z released";
        break;
    case Qt::Key_H: m_camera.modifyPanVector(glm::vec3(0.0f, 0.01f, 0.0f));
        qDebug() << "Key  h released";
        break;
    }
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
    return m_camera;
}