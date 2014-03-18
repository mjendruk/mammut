#include "BunchOfTets.h"

#include "Tet.h"

BunchOfTets::BunchOfTets()
:   m_zShift(0.0f)
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
