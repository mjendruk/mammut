#pragma once

#include <glm/glm.hpp>

#include <LinearMath/btVector3.h>

namespace Conversions
{

glm::vec3 toGlmVec3(const btVector3 & vec3);

}