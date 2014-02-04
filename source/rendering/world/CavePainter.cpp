#include "CavePainter.h"

#include <glm/gtx/transform.hpp>

#include <glow/Program.h>
#include <glow/Shader.h>

#include "DrawableInterface.h"
#include "FileAssociatedShader.h"

CavePainter::CavePainter()
:   m_program(nullptr)
{
}

CavePainter::~CavePainter()
{
    delete m_program;
}

bool CavePainter::initialize()
{
    m_program = new glow::Program();

    glow::Shader * m_fragShader = FileAssociatedShader::getOrCreate(
        GL_FRAGMENT_SHADER, "data/shaders/cuboid.frag", *m_program);
    glow::Shader * m_vertShader = FileAssociatedShader::getOrCreate(
        GL_VERTEX_SHADER, "data/shaders/cuboid.vert", *m_program);
    m_program->link();

    return true;
}

void CavePainter::setNearFarUniform(const glm::vec2 & nearFar)
{
    m_program->setUniform("nearFar", nearFar);
}

void CavePainter::setViewProjectionUniform(const glm::mat4 & viewProjection)
{
    m_program->setUniform("viewProjection", viewProjection);
}

void CavePainter::setViewUniform(const glm::mat4 & view)
{
    m_program->setUniform("view", view);
}

void CavePainter::setEyeUniform(const glm::vec3 & eye)
{
    m_program->setUniform("eye", eye);
}

void CavePainter::update(const QList<glow::Program *> & programs)
{
    //do necessary updates
}

void CavePainter::paint(DrawableInterface & drawable, const glm::mat4 & modelMatrix)
{
    m_program->setUniform("model", modelMatrix);
    drawable.draw();
}
