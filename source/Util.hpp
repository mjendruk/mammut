#pragma once

namespace Util
{
    
template <typename genType>
glm::detail::tvec3<genType> toCartesian(const glm::detail::tvec4<genType> & homogenousVec)
{
    return homogenousVec.xyz() / homogenousVec.w;
}
    
} // namespace