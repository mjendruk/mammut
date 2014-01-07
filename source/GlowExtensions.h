#pragma once

#include <glm/glm.hpp>
#include <glowutils/AxisAlignedBoundingBox.h>

namespace GlowExtensions
{

glowutils::AxisAlignedBoundingBox transformedBoundingBox(const glowutils::AxisAlignedBoundingBox & boundingBox,
                                                         const glm::mat4 & transformation);
}
