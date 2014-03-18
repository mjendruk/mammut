#pragma once

#include <glm/glm.hpp>

#include <QVector>

#include <btBulletDynamicsCommon.h>

#include "../PhysicsObject.h"

class btRigidBody;

class Tet : public PhysicsObject
{
public:
    Tet(QVector<glm::vec3> & vertices);
    ~Tet();

public:
    const QVector<glm::vec3> & duplicatedVertices() const;
    const QVector<glm::vec3> & normals() const;
    glm::mat4 modelMatrix() const;

protected:
    static QVector<glm::vec3> buildDuplicatedVertices(const QVector<glm::vec3> & vertices);
    static void correctFaceOrientation(QVector<glm::vec3> & tri, const glm::vec3 & tetCenter);
    
     void initializeRigidBody();

protected:
    const glm::vec3 m_center;
    const QVector<glm::vec3> m_vertices;
    const QVector<glm::vec3> m_duplicatedVertices;
    const QVector<glm::vec3> m_normals;
    
    
    glm::mat4 m_modelMatrix;
};
