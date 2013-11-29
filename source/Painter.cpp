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
        GL_FRAGMENT_SHADER, "data/default.frag", *m_program);
    glow::Shader * m_vertShader = FileAssociatedShader::getOrCreate(
        GL_VERTEX_SHADER, "data/default.vert", *m_program);
    m_program->link();

    return true;
}

void Painter::setTransformUniform(const glm::mat4 & viewProjection)
{
    m_program->setUniform("transform", viewProjection);
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
