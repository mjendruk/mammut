#pragma once

#include <vector>

#include <glm/glm.hpp>

#include <QVector>

class Tet;

class BunchOfTets
{
public:
    BunchOfTets();
    ~BunchOfTets();

    void add(Tet & tet);
    void add(const QVector<Tet *> * tets);

    void collectVertices(std::vector<glm::vec3> & vertices) const;
    void collectNormals(std::vector<glm::vec3> & vertices) const;
    void collectMatrices(std::vector<glm::mat4> & vertices) const;

protected:
    QVector<Tet *> m_tets;
    
};