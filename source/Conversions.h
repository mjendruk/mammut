#pragma once

#include <glm/glm.hpp>

#include <LinearMath/btVector3.h>
#include <LinearMath/btQuaternion.h>

namespace Conversions
{

glm::vec3 toGlmVec3(const btVector3 & vec3);
glm::mat4 toGlmMat4(const btQuaternion & quat);
glm::mat4 toGlmMat4(const btVector3 & vec3);

}