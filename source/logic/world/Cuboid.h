#pragma once

#include <glm/glm.hpp>

#include "PhysicsObject.h"

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

protected:
    void initializeRigidBody(const glm::vec3 & size, const glm::vec3 & translation);

protected:
    glm::vec3 m_size;
};
