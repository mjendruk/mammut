#include "Painter.h"

#include <glm/gtx/transform.hpp>

#include <glow/Program.h>
#include <glow/Shader.h>

#include "Drawable.h"
#include "FileAssociatedShader.h"

Painter::Painter(Drawable & cuboidDrawable)
:   m_cuboidDrawable(cuboidDrawable)
,   m_program(nullptr)
{
}

Painter::~Painter()
{
    delete m_program;
}

bool Painter::initialize()
{
    m_program = new glow::Program();

    glow::Shader * m_fragShader = FileAssociatedShader::getOrCreate(
        GL_FRAGMENT_SHADER, "data/cuboid.frag", *m_program);
    glow::Shader * m_vertShader = FileAssociatedShader::getOrCreate(
        GL_VERTEX_SHADER, "data/cuboid.vert", *m_program);
    m_program->link();

    return true;
}

void Painter::setNearFarUniform(const glm::vec2 & nearFar)
{
    m_program->setUniform("nearFar", nearFar);
}

void Painter::setViewProjectionUniform(const glm::mat4 & viewProjection)
{
    m_program->setUniform("viewProjection", viewProjection);
}

void Painter::setViewUniform(const glm::mat4 & view)
{
    m_program->setUniform("view", view);
}

void Painter::setEyeUniform(const glm::vec3 & eye)
{
    m_program->setUniform("eye", eye);
}

void Painter::update(const QList<glow::Program *> & programs)
{
    //do necessary updates
}

void Painter::paint(Drawable & drawable, const glm::mat4 & modelMatrix)
{
    m_program->setUniform("model", modelMatrix);
    drawable.draw();
}
