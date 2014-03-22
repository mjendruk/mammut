#include "TessCuboidDrawable.h"

#include <vector>

#include <glm/glm.hpp>

#include <QDebug>

#include <glow/VertexArrayObject.h>
#include <glow/Buffer.h>
#include <glow/VertexAttributeBinding.h>

#include <logic/world/Cuboid.h>
#include <Util.h>

TessCuboidDrawable::TessCuboidDrawable(const Cuboid * cuboid)
:   m_cuboid(cuboid)
{
    initialize();
}

TessCuboidDrawable::~TessCuboidDrawable()
{
}

void TessCuboidDrawable::draw()
{
    m_vao->bind();
    m_vao->drawArrays(GL_TRIANGLES, 0, m_cuboid->hullVertices()->size());
    m_vao->unbind();
}

const Cuboid * TessCuboidDrawable::cuboid()
{
    return m_cuboid;
}

void TessCuboidDrawable::initialize()
{
    const GLuint vertexAttribLocation = 0;
    const GLuint normalAttribLocation = 1;

    m_vao = new glow::VertexArrayObject();

    m_vertexBuffer = new glow::Buffer(GL_ARRAY_BUFFER);
    std::vector<glm::vec3> vertices = m_cuboid->hullVertices()->toStdVector();
    m_vertexBuffer->setData(vertices);
    m_normalBuffer = new glow::Buffer(GL_ARRAY_BUFFER);
    std::vector<glm::vec3> normals = Util::generateNormals(*(m_cuboid->hullVertices())).toStdVector();
    m_normalBuffer->setData(normals);

    m_vao->bind();

    auto vertexBinding = m_vao->binding(0);
    vertexBinding->setAttribute(vertexAttribLocation);
    vertexBinding->setBuffer(m_vertexBuffer, 0, sizeof(glm::vec3));
    vertexBinding->setFormat(3, GL_FLOAT, GL_FALSE);
    m_vao->enable(0);

    auto normalBinding = m_vao->binding(1);
    normalBinding->setAttribute(normalAttribLocation);
    normalBinding->setBuffer(m_normalBuffer, 0, sizeof(glm::vec3));
    normalBinding->setFormat(3, GL_FLOAT, GL_TRUE);
    m_vao->enable(1);

    m_vao->unbind();
}
