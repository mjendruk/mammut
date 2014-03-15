#pragma once

#include <glm/glm.hpp>

#include <QVector>

class Tet
{
public:
    Tet(const QVector<glm::vec3> & vertices);
    ~Tet();

public:
    const QVector<glm::vec3> & duplicatedVertices() const;
    const QVector<glm::vec3> & normals() const;
    glm::mat4 modelMatrix() const;

protected:
    static QVector<glm::vec3> buildDuplicatedVertices(const QVector<glm::vec3> & vertices, const glm::vec3 & verticesCenter);
    static void correctFaceOrientation(QVector<glm::vec3> & tri, const glm::vec3 & tetCenter);
    
    void initializeRigidBody();

protected:
    const QVector<glm::vec3> m_vertices;
    const glm::vec3 m_center;
    const QVector<glm::vec3> m_duplicatedVertices;
    const QVector<glm::vec3> m_normals;
    
    glm::mat4 m_modelMatrix;
};