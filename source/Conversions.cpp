#include "Conversions.h"

namespace Conversions
{

glm::vec3 toGlmVec3(const btVector3 & vec3)
{
	return glm::vec3(vec3.x(), vec3.y(), vec3.z());
}

}