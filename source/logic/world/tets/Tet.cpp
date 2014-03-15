#include "Tet.h"

#include <Util.h>

Tet::Tet(const QVector<glm::vec3> & vertices)
:   m_vertices(vertices)
,   m_center(Util::center(m_vertices))
,   m_duplicatedVertices(buildDuplicatedVertices(m_vertices, m_center))
,   m_normals(Util::generateNormals(m_duplicatedVertices))
,   m_modelMatrix(glm::mat4())
{

}

Tet::~Tet()
{

}


QVector<glm::vec3> Tet::buildDuplicatedVertices(const QVector<glm::vec3> & vertices, const glm::vec3 & verticesCenter)
{
    assert(Util::center(vertices) == verticesCenter);

    QVector<glm::vec3> duplicatedVertices;
    for (int j = 0; j < 4; j++) {
        QVector<glm::vec3> tri;
        for (int k = 0; k < 4; k++) {
            if (k == j)
                continue;
            tri << vertices[k];
        }
        correctFaceOrientation(tri, verticesCenter);

        duplicatedVertices << tri;
    }

    return duplicatedVertices;
}

void Tet::correctFaceOrientation(QVector<glm::vec3> & tri, const glm::vec3 & tetCenter)
{
    glm::vec3 triCenter = Util::center(tri);

    //if the normal is pointing in the wrong direction, reverse the vertices order
    glm::vec3 normal = Util::normalOfTriangle(tri);
    normal *= 0.0001;

    if (glm::distance(triCenter, tetCenter) > glm::distance(triCenter + normal, tetCenter))
        std::swap(tri[0], tri[2]);
}

void Tet::initializeRigidBody()
{

}

const QVector<glm::vec3> & Tet::duplicatedVertices() const
{
    return m_duplicatedVertices;
}

const QVector<glm::vec3> & Tet::normals() const
{
    return m_normals;
}

glm::mat4 Tet::modelMatrix() const
{
    return m_modelMatrix;
}

