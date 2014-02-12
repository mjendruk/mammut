#include "Util.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glow/Texture.h>


namespace Util
{

VectorAxes maxAxis(const glm::vec3 & vec)
{
    return vec.x < vec.y ? (vec.y < vec.z ? kZAxis : kYAxis) : (vec.x < vec.z ? kZAxis : kXAxis);
}

glm::vec3 toGlmVec3(const btVector3 & vec3)
{
    return glm::vec3(vec3.x(), vec3.y(), vec3.z());
}

glm::mat4 toGlmMat4(const btQuaternion & quat)
{
    return glm::rotate(glm::degrees(quat.getAngle()),
                       quat.getAxis().x(),
                       quat.getAxis().y(),
                       quat.getAxis().z());
}

glm::mat4 toGlmMat4(const btVector3 & vec3)
{
    return glm::translate(vec3.x(), vec3.y(), vec3.z());
}
    
btVector3 toBtVec3(const glm::vec3 & vec3)
{
    return btVector3(vec3.x, vec3.y, vec3.z);
}
    
btMatrix3x3 toBtMat3(const glm::mat3 & mat3)
{
    return btMatrix3x3(toBtVec3(glm::row(mat3, 0)),
                       toBtVec3(glm::row(mat3, 1)),
                       toBtVec3(glm::row(mat3, 2)));
}

glow::Texture * create2DTexture()
{
    glow::Texture * texture = new glow::Texture(GL_TEXTURE_2D);
    texture->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    texture->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    texture->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    texture->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    texture->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    return texture;
}

} // namespace
