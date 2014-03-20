#pragma once

#include <gl/glew.h>

#include <glm/glm.hpp>

#include <LinearMath/btVector3.h>
#include <LinearMath/btQuaternion.h>
#include <LinearMath/btMatrix3x3.h>

#include <QVector>

namespace glow
{
    class Texture;
}

namespace Util
{

enum VectorAxes { kXAxis, kYAxis, kZAxis };

VectorAxes maxAxis(const glm::vec3 & vec);

glm::vec3 toGlmVec3(const btVector3 & vec3);
glm::mat4 toGlmMat4(const btQuaternion & quat);
glm::mat4 toGlmMat4(const btVector3 & vec3);
    
btVector3 toBtVec3(const glm::vec3 & vec3);
btMatrix3x3 toBtMat3(const glm::mat3 & mat3);

glm::vec3 center(const glm::vec3 & v0, const glm::vec3 & v1, const glm::vec3 & v2);
glm::vec3 center(const QVector<glm::vec3> & vecs);
QVector<glm::vec3> centerVertices(QVector<glm::vec3> & vecs, const glm::vec3 & center);

void correctFaceOrientation(QVector<glm::vec3> & tri, const glm::vec3 & tetCenter);

glm::vec3 normalOfTriangle(const glm::vec3 & v0, const glm::vec3 & v1, const glm::vec3 & v2);
glm::vec3 normalOfTriangle(const QVector<glm::vec3> & triangle);

void generateNormals(const std::vector<glm::vec3> & triangles, std::vector<glm::vec3> & normals);
QVector<glm::vec3> generateNormals(const QVector<glm::vec3> & triangles);

    
template <typename genType>
glm::detail::tvec3<genType> toCartesian(const glm::detail::tvec4<genType> & homogenousVec);

glow::Texture * create2DTexture(GLint filter, GLint wrapMode);

} // namespace

#include "Util.hpp"
