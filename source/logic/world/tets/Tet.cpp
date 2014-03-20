#include "Tet.h"

#include <glm/gtx/transform.hpp>

#include <Util.h>

Tet::Tet(QVector<glm::vec3> & vertices)
:   m_center(Util::center(vertices))
,   m_vertices(Util::centerVertices(vertices, m_center))
,   m_duplicatedVertices(buildDuplicatedVertices(m_vertices))
,   m_normals(Util::generateNormals(m_duplicatedVertices))
,   m_modelMatrix(glm::mat4())
{
    initializeRigidBody();
}

Tet::~Tet()
{

}


QVector<glm::vec3> Tet::buildDuplicatedVertices(const QVector<glm::vec3> & vertices)
{
    assert(glm::distance(glm::vec3(0.0f, 0.0f, 0.0f), Util::center(vertices)) < 0.001);

    glm::vec3 verticesCenter = glm::vec3(0.0f, 0.0f, 0.0f);

    QVector<glm::vec3> duplicatedVertices;
    for (int j = 0; j < 4; j++) {
        QVector<glm::vec3> tri;
        for (int k = 0; k < 4; k++) {
            if (k == j)
                continue;
            tri << vertices[k];
        }
        Util::correctFaceOrientation(tri, verticesCenter);

        duplicatedVertices << tri;
    }

    return duplicatedVertices;
}

void Tet::initializeRigidBody()
{
    assert(m_vertices.size() == 4);
    assert(glm::distance(glm::vec3(0.0f, 0.0f, 0.0f), Util::center(m_vertices)) < 0.001);

    btConvexHullShape * collisionShape = new btConvexHullShape();
    for (const glm::vec3 & vec : m_vertices) {
        collisionShape->addPoint(Util::toBtVec3(vec));
    }
    btMotionState * motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1),
        Util::toBtVec3(m_center)));

    double mass = 2.0;
    btVector3 fallInertia;
    collisionShape->calculateLocalInertia(mass, fallInertia);
    btRigidBody::btRigidBodyConstructionInfo info(mass,
        motionState,
        collisionShape,
        fallInertia);

    info.m_restitution = 0.0f;

    m_collisionShape.reset(collisionShape);
    m_motionState.reset(motionState);

    m_rigidBody.reset(new btRigidBody(info));
    m_rigidBody->setUserPointer(this);
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
    glm::mat4 mat(1.0);
    mat *= glm::translate(Util::toGlmVec3(m_rigidBody->getWorldTransform().getOrigin()));
    mat *= Util::toGlmMat4(m_rigidBody->getWorldTransform().getRotation());
    return mat;
}
