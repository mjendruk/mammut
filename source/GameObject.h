
#pragma once

#include <glm/glm.hpp>

namespace glowutils
{
    class AxisAlignedBoundingBox;
}

class GameObject
{
public:
    GameObject();
    virtual ~GameObject();
    
    virtual glm::mat4 modelTransform() const = 0;
    virtual glowutils::AxisAlignedBoundingBox boundingBox() const = 0;

};
