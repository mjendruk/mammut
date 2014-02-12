#include "Painter.h"

#include <glm/gtx/transform.hpp>

#include <glow/Program.h>
#include <glow/Shader.h>
#include <glowutils/File.h>

#include "DrawableInterface.h"

Painter::Painter()
:   m_program(nullptr)
{
    initialize();
}

Painter::~Painter()
{
}

void Painter::initialize()
{
    m_program = new glow::Program();

    glow::Shader * m_fragShader = glowutils::createShaderFromFile(
 	    GL_FRAGMENT_SHADER, "data/shaders/cuboid.frag");
    glow::Shader * m_vertShader = glowutils::createShaderFromFile(
 	    GL_VERTEX_SHADER, "data/shaders/cuboid.vert");
    m_program->attach(m_vertShader, m_fragShader);
    m_program->link();
}

void Painter::setNearFarUniform(const glm::vec2 & nearFar)
{
    m_program->setUniform("nearFar", nearFar);
}

void Painter::setViewProjectionUniforms(const glm::mat4 & viewProjection, const glm::mat4 & prevViewProjection)
{
    m_program->setUniform("viewProjection", viewProjection);
    m_program->setUniform("prevViewProjection", prevViewProjection);
}

void Painter::setViewUniform(const glm::mat4 & view)
{
    m_program->setUniform("view", view);
}

void Painter::setEyeUniform(const glm::vec3 & eye)
{
    m_program->setUniform("eye", eye);
}

void Painter::paint(DrawableInterface & drawable, const glm::mat4 & modelMatrix, const glm::mat4 & prevModelMatrix)
{
    m_program->setUniform("model", modelMatrix);
    m_program->setUniform("prevModel", prevModelMatrix);
    drawable.draw();
    m_program->release();
}
