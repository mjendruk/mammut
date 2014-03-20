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

glm::vec3 center(const glm::vec3 & v0, const glm::vec3 & v1, const glm::vec3 & v2)
{
    return v0 + v1 + v2 / 3.0f;
}

glm::vec3 center(const QVector<glm::vec3> & vecs)
{
    glm::vec3 center;
    for (glm::vec3 curr : vecs)
        center += curr;
    center /= vecs.size();
    return center;
}

QVector<glm::vec3> centerVertices(QVector<glm::vec3> & vecs, const glm::vec3 & center)
{
    for (int i = 0; i < vecs.size(); i++)
        vecs[i] = vecs[i] - center;
    return vecs;
}

void correctFaceOrientation(QVector<glm::vec3> & tri, const glm::vec3 & tetCenter)
{
    glm::vec3 triCenter = Util::center(tri);

    //if the normal is pointing in the wrong direction, reverse the vertices order
    glm::vec3 normal = Util::normalOfTriangle(tri);
    normal *= 0.0001;

    if (glm::distance(triCenter, tetCenter) > glm::distance(triCenter + normal, tetCenter))
        std::swap(tri[0], tri[2]);
}

glm::vec3 normalOfTriangle(const glm::vec3 & v0, const glm::vec3 & v1, const glm::vec3 & v2)
{
    glm::vec3 ab = v1 - v0;
    glm::vec3 ac = v2 - v0;
    return glm::normalize(glm::cross(ab, ac));
}

glm::vec3 normalOfTriangle(const QVector<glm::vec3> & triangle)
{
    assert(triangle.size() == 3);
    return normalOfTriangle(triangle[0], triangle[1], triangle[2]);
}

void generateNormals(const std::vector<glm::vec3> & triangles, std::vector<glm::vec3> & normals)
{
    assert(triangles.size() % 3 == 0);
    assert(normals.empty());
    for (int i = 0; i < triangles.size(); i += 3) {
        glm::vec3 normal = normalOfTriangle(triangles[i], triangles[i + 1], triangles[i + 2]);
        normals.push_back(normal);
        normals.push_back(normal);
        normals.push_back(normal);
    }
}

QVector<glm::vec3> generateNormals(const QVector<glm::vec3> & triangles)
{
    assert(triangles.size() % 3 == 0);
    QVector<glm::vec3> normals;
    for (int i = 0; i < triangles.size(); i += 3) {
        glm::vec3 normal = normalOfTriangle(triangles[i], triangles[i + 1], triangles[i + 2]);
        normals.push_back(normal);
        normals.push_back(normal);
        normals.push_back(normal);
    }
    return normals;
}

glow::Texture * create2DTexture(GLint filter, GLint wrapMode)
{
    glow::Texture * texture = new glow::Texture(GL_TEXTURE_2D);
    texture->setParameter(GL_TEXTURE_MIN_FILTER, filter);
    texture->setParameter(GL_TEXTURE_MAG_FILTER, filter);
    texture->setParameter(GL_TEXTURE_WRAP_S, wrapMode);
    texture->setParameter(GL_TEXTURE_WRAP_T, wrapMode);
    texture->setParameter(GL_TEXTURE_WRAP_R, wrapMode);
    return texture;
}

} // namespace
