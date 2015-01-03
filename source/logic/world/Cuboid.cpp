#include "Cuboid.h"

#include <glm/gtx/transform.hpp>
#include <glowutils/AxisAlignedBoundingBox.h>
#include <btBulletDynamicsCommon.h>

#include "tets/Tet.h"
#include "tets/BunchOfTets.h"
#include <Util.h>


Cuboid::Cuboid(const glm::vec3 & size, const glm::vec3 & translation)
:   m_size(size)
,   m_containsBoost(false)
,   m_isDummy(false)
,   m_tets(nullptr)
,   m_hullVertices(nullptr)
{    
    initializeRigidBody(size, translation);
}

Cuboid::~Cuboid()
{
    if (m_tets != nullptr)
        qDeleteAll(*m_tets);
    delete m_tets;
    delete m_hullVertices;
}

glm::mat4 Cuboid::modelTransform() const
{
    assert(!m_isDummy);
    return glm::translate(position()) * glm::scale(m_size);
}

glowutils::AxisAlignedBoundingBox Cuboid::boundingBox() const
{
    btVector3 llf, urb;
    m_rigidBody->getAabb(llf, urb);
    
    return glowutils::AxisAlignedBoundingBox(Util::toGlmVec3(llf), Util::toGlmVec3(urb));
}

void Cuboid::addBoost()
{
    m_containsBoost = true;
}

bool Cuboid::containsBoost() const
{
    return m_containsBoost;
}

void Cuboid::collectBoost() const
{
    assert(!m_isDummy);
    assert(containsBoost());
    m_containsBoost = false;
}

BunchOfTets * Cuboid::splitIntoTets()
{
    assert(!m_isDummy);
    for (Tet * tet : *m_tets)
        tet->translate(position());
    BunchOfTets * bunch = new BunchOfTets();
    bunch->add(m_tets);
    delete m_tets;
    m_tets = nullptr;
    m_isDummy = true;

    return bunch;
}

void Cuboid::setTets(QVector<Tet *> * tets)
{
    m_tets = tets;
}

const QVector<glm::vec3> * Cuboid::hullVertices() const
{
    assert(!m_isDummy);
    return m_hullVertices;
}

void Cuboid::setHullVertices(QVector<glm::vec3> * hullVertices)
{
    m_hullVertices = hullVertices;
}

bool Cuboid::tetsReady() const
{
    return m_tets != nullptr;
}

bool Cuboid::isDummy() const
{
    return m_isDummy;
}

void Cuboid::initializeRigidBody(const glm::vec3 & size, const glm::vec3 & translation)
{
    m_collisionShape.reset(new btBoxShape(Util::toBtVec3(size / 2.0f)));
    
    m_motionState.reset(new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1),
                                                             Util::toBtVec3(translation))));
    
    btRigidBody::btRigidBodyConstructionInfo info(0, m_motionState.get(), m_collisionShape.get());
    
    info.m_restitution = 0.0f;
    
    m_rigidBody.reset(new btRigidBody(info));
    m_rigidBody->setUserPointer(this);
}
