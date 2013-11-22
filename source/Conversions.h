#pragma once

#include <glm/glm.hpp>
#include <btBulletDynamicsCommon.h>

namespace Conversions
{
	glm::vec3 toGlmVec3(const btVector3 & vec3);
}