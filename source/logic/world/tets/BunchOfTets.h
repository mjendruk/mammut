#pragma once

#include <vector>

#include <glm/glm.hpp>

#include <QVector>

class Tet;
class PhysicsWorld;

class BunchOfTets
{
public:
    BunchOfTets();
    ~BunchOfTets();

    void add(Tet & tet);
    void add(const QVector<Tet *> * tets);

    void update(float seconds, PhysicsWorld & physicsWorld);

    void collectVertices(std::vector<glm::vec3> & vertices) const;
    void collectNormals(std::vector<glm::vec3> & vertices) const;
    void collectMatrices(std::vector<glm::mat4> & vertices) const;

    void addZShift(float zShift);
    float zShift() const;

protected:
    QVector<Tet *> m_tets;
    int m_physicsTetIndex;

    float m_zShift;
};
