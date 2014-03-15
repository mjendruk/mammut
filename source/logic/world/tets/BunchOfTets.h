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

    std::vector<glm::vec3> * collectVertices() const;
    std::vector<glm::vec3> * collectNormals() const;
    std::vector<glm::mat4> * collectMatrices() const;

protected:
    QVector<Tet *> m_tets;
    
};