#include "GameLogic.h"

#include <glm/gtx/transform.hpp>

#include <QDebug>
#include <QThread>
#include <QVector>

#include <btBulletDynamicsCommon.h>

#include "Cuboid.h"

GameLogic::GameLogic()
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
    glm::mat4 mat = glm::translate(0.f, 10.0f, 0.0f);

    m_cuboids << new Cuboid(glm::vec3(.3, .5f, 2.f));
    m_cuboids << new Cuboid(glm::vec3(.5f, .5f, 1.f), mat);

    btBroadphaseInterface* broadphase = new btDbvtBroadphase();
    btDefaultCollisionConfiguration * collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher * dispatcher = new btCollisionDispatcher(collisionConfiguration);
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();
    m_dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
    m_dynamicsWorld->setGravity(btVector3(0, -9.81, 0));

    btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);

    btCollisionShape * fallShape = new btBoxShape(btVector3(.5f, .5f, 1.f));

    btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));

    btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
    btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);

    m_dynamicsWorld->addRigidBody(groundRigidBody);

    btDefaultMotionState* fallMotionState =
        new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 10, 0)));

    btScalar mass = 1;
    btVector3 fallInertia(0, 0, 0);
    fallShape->calculateLocalInertia(mass, fallInertia);

    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape, fallInertia);
    m_fallRigidBody = new btRigidBody(fallRigidBodyCI);
    m_dynamicsWorld->addRigidBody(m_fallRigidBody);

}

void GameLogic::update(int ms)
{
    QThread::msleep(2);

    m_dynamicsWorld->stepSimulation(1 / 200.f);

    btTransform transform;
    m_fallRigidBody->getMotionState()->getWorldTransform(transform);
    btVector3 origin = transform.getOrigin();
    btQuaternion quat = transform.getRotation();

    glm::mat4 mat;
    mat *= glm::translate(origin.x(), origin.y(), origin.z());
    mat *= glm::rotate(quat.getAngle(), quat.getAxis().x(), quat.getAxis().y(), quat.getAxis().z());
    m_cuboids.at(1)->setMatrix(mat);

}

void GameLogic::keyPressed(int key)
{
    switch (key)
    {
    case Qt::Key_W: m_fallRigidBody->setLinearVelocity(m_fallRigidBody->getLinearVelocity() + btVector3(0, 0, 5));
                    qDebug() << "Key  w pressed";
                    break;
    case Qt::Key_A: qDebug() << "Key  a pressed";
                    break;
    case Qt::Key_S: qDebug() << "Key  s pressed";
                    break;
    case Qt::Key_D: qDebug() << "Key  d pressed";
                    break;
    }
}

void GameLogic::keyReleased(int key)
{
    switch (key)
    {
    case Qt::Key_W: m_fallRigidBody->setLinearVelocity(m_fallRigidBody->getLinearVelocity() - btVector3(0, 0, 5));
                    qDebug() << "Key  w released";
                    break;
    case Qt::Key_A: qDebug() << "Key  a released";
        break;
    case Qt::Key_S: qDebug() << "Key  s released";
        break;
    case Qt::Key_D: qDebug() << "Key  d released";
        break;
    }
}