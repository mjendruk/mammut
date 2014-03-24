#pragma once

#include <glm/glm.hpp>

#include <QVector>

#include <btBulletDynamicsCommon.h>

#include <logic/world/PhysicsObject.h>


class btRigidBody;

class Tet : public PhysicsObject
{
public:
    Tet(QVector<glm::vec3> & vertices);
    ~Tet();

    const QVector<glm::vec3> & duplicatedVertices() const;
    const QVector<glm::vec3> & normals() const;
    glm::mat4 modelMatrix() const;
    
    void scale(float factor);

protected:
    static QVector<glm::vec3> buildDuplicatedVertices(const QVector<glm::vec3> & vertices);
    
     void initializeRigidBody();

protected:
    const glm::vec3 m_center;
    const QVector<glm::vec3> m_vertices;
    const QVector<glm::vec3> m_duplicatedVertices;
    const QVector<glm::vec3> m_normals;

    float m_scaleFactor;
    
    
    glm::mat4 m_modelMatrix;
};
