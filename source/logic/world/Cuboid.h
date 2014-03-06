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

    const glm::mat4 & modelTransform() const;

    glowutils::AxisAlignedBoundingBox boundingBox() const;

protected:
    void initializeRigidBody(const glm::vec3 & size, const glm::vec3 & translation);
    void initializeModelTransform();

protected:

    glm::mat4 m_modelTransform;
    glm::vec3 m_size;
};
