#pragma once

#include <glm/glm.hpp>

#include <LinearMath/btVector3.h>
#include <LinearMath/btQuaternion.h>
#include <LinearMath/btMatrix3x3.h>

namespace Util
{

enum VectorAxes { kXAxis, kYAxis, kZAxis };

VectorAxes maxAxis(const glm::vec3 & vec);

glm::vec3 toGlmVec3(const btVector3 & vec3);
glm::mat4 toGlmMat4(const btQuaternion & quat);
glm::mat4 toGlmMat4(const btVector3 & vec3);
    
btVector3 toBtVec3(const glm::vec3 & vec3);
btMatrix3x3 toBtMat3(const glm::mat3 & mat3);

} // namespace Util
