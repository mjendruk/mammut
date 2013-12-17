#include "GlowExtensions.h"

namespace GlowExtensions
{
    
glowutils::AxisAlignedBoundingBox transformedBoundingBox(
    const glowutils::AxisAlignedBoundingBox & boundingBox, 
    const glm::mat4 & transformation)
{
    glowutils::AxisAlignedBoundingBox transformedBoundingBox;

    glm::vec4 llf = transformation * glm::vec4(boundingBox.llf(), 1.0f);
    glm::vec4 urb = transformation * glm::vec4(boundingBox.urb(), 1.0f);

    transformedBoundingBox.extend(llf.xyz() / llf.w);
    transformedBoundingBox.extend(urb.xyz() / urb.w);

    return transformedBoundingBox;
}

}
