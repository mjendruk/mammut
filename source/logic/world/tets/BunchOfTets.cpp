#include "BunchOfTets.h"

#include "Tet.h"

BunchOfTets::BunchOfTets()
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


std::vector<glm::vec3> * BunchOfTets::collectVertices() const
{
    std::vector<glm::vec3> * vertices = new std::vector<glm::vec3>();
    for (Tet * tet: m_tets) {
        for (glm::vec3 vec : tet->duplicatedVertices())
            vertices->push_back(vec);
    }
    return vertices;
}

std::vector<glm::vec3> * BunchOfTets::collectNormals() const
{
    std::vector<glm::vec3> * normals = new std::vector<glm::vec3>();
    for (Tet * tet : m_tets) {
        for (glm::vec3 vec : tet->normals())
            normals->push_back(vec);
    }
    return normals;
}

std::vector<glm::mat4> * BunchOfTets::collectMatrices() const
{
    std::vector<glm::mat4> * matrices = new std::vector<glm::mat4>();
    for (Tet * tet : m_tets) {
        matrices->push_back(tet->modelMatrix());
    }
    return matrices;
}