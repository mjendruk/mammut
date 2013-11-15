#include "Painter.h"

#include <cassert>

#include <glm/gtx/transform.hpp>

#include <glow/Program.h>
#include <glow/Shader.h>
#include <glowutils/Icosahedron.h>

#include <btBulletDynamicsCommon.h>

#include "FileAssociatedShader.h"
#include "Camera.h"
#include "Quad.h"

Painter::Painter()
:   m_program(nullptr)
,   m_fragShader(nullptr)
,   m_vertShader(nullptr)
{
}

Painter::~Painter()
{
    delete m_program;

    delete m_fragShader;
    delete m_vertShader;
    
    qDeleteAll(m_quads);
}

bool Painter::initialize()
{
    glClearColor(1.f, 1.f, 1.f, 0.f);

    m_program = new glow::Program();

    m_fragShader = FileAssociatedShader::getOrCreate(
        GL_FRAGMENT_SHADER, "data/default.frag", *m_program);
    m_vertShader = FileAssociatedShader::getOrCreate(
        GL_VERTEX_SHADER, "data/default.vert", *m_program);
    m_program->link();
    
    glm::mat4 mat = glm::translate(0.f, 10.0f, 0.0f);
    //mat *= glm::rotate(45.f, 0.f, 0.f, 1.f);
    
    glm::mat4 mat2;

    m_quads << new Quad(glm::vec3(.3, .5f, 2.f), mat2);
    m_quads << new Quad(glm::vec3(.5f, .5f, 1.f), mat);
    
    btBroadphaseInterface* broadphase = new btDbvtBroadphase();
    btDefaultCollisionConfiguration * collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher * dispatcher = new btCollisionDispatcher(collisionConfiguration);
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();
    m_dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
    m_dynamicsWorld->setGravity(btVector3(0, -9.81, 0));
    
    btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),1);
    
    btCollisionShape * fallShape = new btBoxShape(btVector3(.5f, .5f, 1.f));
    
    btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-1,0)));
    
    btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
    btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
    
    m_dynamicsWorld->addRigidBody(groundRigidBody);
    
    btDefaultMotionState* fallMotionState =
                new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,10,0)));
    
    btScalar mass = 1;
        btVector3 fallInertia(0,0,0);
        fallShape->calculateLocalInertia(mass,fallInertia);
    
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState,fallShape,fallInertia);
    m_fallRigidBody = new btRigidBody(fallRigidBodyCI);
    m_dynamicsWorld->addRigidBody(m_fallRigidBody);
    
    
    return true;
}

void Painter::resize(
    int width
,   int height)
{
    glViewport(0, 0, width, height);

    if (m_program->isLinked())
    {
        m_program->use();
        m_program->setUniform("transform", camera()->viewProjection());
        m_program->release();
    }
}

void Painter::update()
{
    if (m_program->isLinked())
    {
        m_program->use();
        m_program->setUniform("transform", camera()->viewProjection());
        m_program->release();
    }
}

void Painter::update(const QList<glow::Program *> & programs)
{
    if (programs.contains(m_program) && m_program->isLinked())
    {
        m_program->use();
        m_program->setUniform("transform", camera()->viewProjection());
        m_program->release();
    }
}

void Painter::paint(float timef)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    m_dynamicsWorld->stepSimulation(1/200.f);
    
    btTransform transform;
    m_fallRigidBody->getMotionState()->getWorldTransform(transform);
    btVector3 origin = transform.getOrigin();
    btQuaternion quat = transform.getRotation();
    
    glm::mat4 mat;
    mat *= glm::translate(origin.x(), origin.y(), origin.z());
    mat *= glm::rotate(quat.getAngle(), quat.getAxis().x(), quat.getAxis().y(), quat.getAxis().z());
    m_quads.at(1)->setMatrix(mat);
    
    if (m_program->isLinked())
    {
        m_program->use();
        for (Quad * quad : m_quads) {
            m_program->setUniform("model", quad->matrix());
            quad->draw();
        }
        m_program->release();
    }
 
}
