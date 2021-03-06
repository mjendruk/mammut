#include "CavePainter.h"

#include <glow/Program.h>
#include <glow/Shader.h>
#include <glowutils/global.h>

#include "DrawableInterface.h"

CavePainter::CavePainter()
:   m_program(nullptr)
{
    initialize();
}

CavePainter::~CavePainter()
{
    delete m_program;
}

void CavePainter::initialize()
{
    m_program = new glow::Program();

    glow::Shader * m_fragShader = glowutils::createShaderFromFile(
	GL_FRAGMENT_SHADER,  "data/shaders/cave.frag");
    glow::Shader * m_vertShader = glowutils::createShaderFromFile(
	GL_VERTEX_SHADER, "data/shaders/cave.vert");
    m_program->attach(m_vertShader, m_fragShader);
    m_program->link();
}

void CavePainter::setViewProjectionUniforms(const glm::mat4 & viewProjection, const glm::mat4 & prevViewProjection)
{
    m_program->setUniform("viewProjection", viewProjection);
    m_program->setUniform("prevViewProjection", prevViewProjection);
}

void CavePainter::setViewUniform(const glm::mat4 & view)
{
    m_program->setUniform("view", view);
}

void CavePainter::paint(DrawableInterface & drawable, const glm::mat4 & modelMatrix, const glm::mat4 & prevModelMatrix)
{
    m_program->setUniform("model", modelMatrix);
    m_program->setUniform("prevModel", prevModelMatrix);
    drawable.draw();
}
