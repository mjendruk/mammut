#include "Painter.h"

#include <cassert>

#include <glm/gtx/transform.hpp>

#include <glow/Program.h>
#include <glow/Shader.h>
#include <glow/VertexArrayObject.h>
#include <glow/Buffer.h>
#include <glow/VertexAttributeBinding.h>

#include "Drawable.h"
#include "FileAssociatedShader.h"
#include "RenderCamera.h"
#include "Cuboid.h"

Painter::Painter(Drawable & cuboidDrawable)
:   m_cuboidDrawable(cuboidDrawable)
,   m_program(nullptr)
,   m_fragShader(nullptr)
,   m_vertShader(nullptr)
{
}

Painter::~Painter()
{
    delete m_program;
    delete m_fragShader;
    delete m_vertShader;
}

bool Painter::initialize()
{
    m_program = new glow::Program();

    m_fragShader = FileAssociatedShader::getOrCreate(
        GL_FRAGMENT_SHADER, "data/default.frag", *m_program);
    m_vertShader = FileAssociatedShader::getOrCreate(
        GL_VERTEX_SHADER, "data/default.vert", *m_program);
    m_program->link();

    return true;
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

void Painter::paint(Drawable & drawable, glm::mat4 & modelMatrix)
{
    if (m_program->isLinked())
    {
        m_program->use();

        m_program->setUniform("model", modelMatrix);
        drawable.draw();
        m_program->release();
    }
}
