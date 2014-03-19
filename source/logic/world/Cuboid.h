#pragma once

#include <glm/glm.hpp>

#include <QVector>

#include "PhysicsObject.h"

class Tet;

namespace glowutils
{
    class AxisAlignedBoundingBox;
}

class Cuboid : public PhysicsObject
{
public:
    Cuboid(const glm::vec3 & size,
           const glm::vec3 & translation);
         
    virtual ~Cuboid();


    glm::mat4 modelTransform() const;

    glowutils::AxisAlignedBoundingBox boundingBox() const;

    void addBoost();
    bool containsBoost() const override;
    void collectBoost() const override;
    
    QVector<Tet *> * tets();
    void setTets(QVector<Tet *> * tets);
    bool tetsReady();

protected:
    void initializeRigidBody(const glm::vec3 & size, const glm::vec3 & translation);

protected:
    glm::vec3 m_size;

    bool mutable m_containsBoost;
    QVector<Tet *> * m_tets;
};
