#include "Conversions.h"

#include <glm/gtx/transform.hpp>

namespace Conversions
{

glm::vec3 toGlmVec3(const btVector3 & vec3)
{
	return glm::vec3(vec3.x(), vec3.y(), vec3.z());
}

glm::mat4 toGlmMat4(const btQuaternion & quat)
{
    return glm::rotate(glm::degrees(quat.getAngle()), quat.getAxis().x(), quat.getAxis().y(), quat.getAxis().z());
}

glm::mat4 toGlmMat4(const btVector3 & vec3)
{
    return glm::translate(vec3.x(), vec3.y(), vec3.z());
}
    
btVector3 toBtVec3(const glm::vec3 & vec3)
{
    return btVector3(vec3.x, vec3.y, vec3.z);
}

} // namespace Conversions