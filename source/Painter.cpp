#include "Painter.h"

#include <cassert>

#include <glm/gtx/transform.hpp>

#include <glow/Program.h>
#include <glow/Shader.h>
#include <glow/VertexArrayObject.h>
#include <glow/Buffer.h>
#include <glow/VertexAttributeBinding.h>

#include "FileAssociatedShader.h"
#include "RenderCamera.h"
#include "Cuboid.h"

Painter::Painter()
: m_initialized(false)
, m_program(nullptr)
, m_fragShader(nullptr)
, m_vertShader(nullptr)
, m_cuboidVao(nullptr)
, m_cuboidVertices(nullptr)
, m_cuboidIndices(nullptr)
{
}

Painter::~Painter()
{
    delete m_program;
    delete m_fragShader;
    delete m_vertShader;
    delete m_cuboidVao;
    delete m_cuboidVertices;
    delete m_cuboidIndices;
}


void Painter::initializeCuboidData()
{
    m_cuboidVao = new glow::VertexArrayObject();
    m_cuboidVertices = new glow::Buffer(GL_ARRAY_BUFFER);
    m_cuboidIndices = new glow::Buffer(GL_ELEMENT_ARRAY_BUFFER);
    auto v(vertices());
    auto i(indices());

    m_cuboidVertices->setData(v, GL_STATIC_DRAW);
    m_cuboidIndices->setData(i, GL_STATIC_DRAW);

    m_cuboidVao->bind();

    auto vertexBinding = m_cuboidVao->binding(0);
    vertexBinding->setAttribute(0);
    vertexBinding->setBuffer(m_cuboidVertices, 0, sizeof(glm::vec3));
    vertexBinding->setFormat(3, GL_FLOAT);
    m_cuboidVao->enable(0);

    m_cuboidIndices->bind();

    m_cuboidVao->unbind();
}

bool Painter::initialize()
{
    glClearColor(1.f, 1.f, 1.f, 0.f);

    m_program = new glow::Program();

    m_fragShader = FileAssociatedShader::getOrCreate(
        GL_FRAGMENT_SHADER, "data/default.frag", *m_program);
    m_vertShader = FileAssociatedShader::getOrCreate(
        GL_VERTEX_SHADER, "data/default.vert", *m_program);
    m_program->link();

    initializeCuboidData();

    return true;
}

void Painter::resize(int width, int height)
{
    glViewport(0, 0, width, height);

    if (m_program->isLinked())
    {
        m_program->use();
        m_program->setUniform("transform", camera()->viewProjection());
        m_program->release();
    }
}

void Painter::update()
{
    if (m_program->isLinked())
    {
        m_program->use();
        m_program->setUniform("transform", camera()->viewProjection());
        m_program->release();
    }
}

void Painter::update(const QList<glow::Program *> & programs)
{
    if (programs.contains(m_program) && m_program->isLinked())
    {
        m_program->use();
        m_program->setUniform("transform", camera()->viewProjection());
        m_program->release();
    }
}

void Painter::paint(float timef)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (m_program->isLinked())
    {
        m_program->use();
        for (Cuboid * cuboid : *m_cuboids) {
            glm::mat4 scaleMatrix = glm::scale(cuboid->size());
            m_program->setUniform("model", cuboid->matrix() * scaleMatrix);
            drawCuboid();
        }
        m_program->release();
    }
}


void Painter::drawCuboid()
{
    glEnable(GL_DEPTH_TEST);

    m_cuboidVao->bind();
    m_cuboidVao->drawElements(GL_TRIANGLES, indices().size() * 3, GL_UNSIGNED_SHORT, nullptr);
    m_cuboidVao->unbind();
}


glow::Vec3Array Painter::vertices()
{
    return glow::Vec3Array
    {
        glm::vec3(0.f, 0.f, 0.f),
        glm::vec3(1.f, 0.f, 0.f),
        glm::vec3(1.f, 1.f, 0.f),
        glm::vec3(0.f, 1.f, 0.f),
        glm::vec3(0.f, 0.f, -1.f),
        glm::vec3(1.f, 0.f, -1.f),
        glm::vec3(1.f, 1.f, -1.f),
        glm::vec3(0.f, 1.f, -1.f),
    };
}

glow::Array<glm::lowp_uvec3> Painter::indices()
{
    return glow::Array<glm::lowp_uvec3>
    {
        glm::lowp_uvec3(0, 1, 2),
        glm::lowp_uvec3(0, 2, 3),
        glm::lowp_uvec3(0, 1, 5),
        glm::lowp_uvec3(0, 5, 4),
        glm::lowp_uvec3(0, 4, 7),
        glm::lowp_uvec3(0, 7, 3),
        glm::lowp_uvec3(6, 5, 1),
        glm::lowp_uvec3(6, 1, 2),
        glm::lowp_uvec3(6, 2, 3),
        glm::lowp_uvec3(6, 3, 7),
        glm::lowp_uvec3(6, 5, 4),
        glm::lowp_uvec3(6, 4, 7)
    };
}
