#include "BunchOfTets.h"

#include <logic/world/PhysicsWorld.h>

#include "Tet.h"


const float BunchOfTets::s_shrinkFactor = 0.95f;

BunchOfTets::BunchOfTets()
:   m_shrinkedTetIndex(0)
,   m_movingTetIndex(0)
,   m_zShift(0.0f)
{

}

BunchOfTets::~BunchOfTets()
{

}


void BunchOfTets::add(Tet & tet)
{
    m_tets << &tet;
}

void BunchOfTets::add(const QVector<Tet *> * tets)
{
    m_tets << *tets;
}

void BunchOfTets::update(float seconds, PhysicsWorld & physicsWorld)
{
    if (m_shrinkedTetIndex < m_tets.size()) {
        m_tets[m_shrinkedTetIndex]->scale(s_shrinkFactor);
        m_shrinkedTetIndex++;
    }

    if (m_shrinkedTetIndex == m_tets.size() && m_movingTetIndex < m_tets.size()) {
        physicsWorld.addObject(m_tets[m_movingTetIndex]);
        m_movingTetIndex++;
    }
}

void BunchOfTets::collectVertices(std::vector<glm::vec3> & vertices) const
{
    assert(vertices.size() == 0);
    for (Tet * tet: m_tets) {
        for (glm::vec3 vec : tet->duplicatedVertices())
            vertices.push_back(vec);
    }
}

void BunchOfTets::collectNormals(std::vector<glm::vec3> & normals) const
{
    assert(normals.size() == 0);
    for (Tet * tet : m_tets) {
        for (glm::vec3 vec : tet->normals())
            normals.push_back(vec);
    }
}

void BunchOfTets::collectMatrices(std::vector<glm::mat4> & matrices) const
{
    assert(matrices.size() == 0);
    for (Tet * tet : m_tets) {
        matrices.push_back(tet->modelMatrix());
    }
}

void BunchOfTets::addZShift(float zShift)
{
    m_zShift += zShift;
}

float BunchOfTets::zShift() const
{
    return m_zShift;
}
