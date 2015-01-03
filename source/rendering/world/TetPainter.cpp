#include "TetPainter.h"

#include <glow/Shader.h>
#include <glow/Program.h>
#include <glowutils/global.h>

#include "BunchOfTetsDrawable.h"


TetPainter::TetPainter()
{
    initialize();
}

TetPainter::~TetPainter()
{

}

void TetPainter::setNearFarUniform(const glm::vec2 & nearFar)
{
    m_program->setUniform("nearFar", nearFar);
}

void TetPainter::setViewProjectionUniform(const glm::mat4 & viewProjection)
{
    m_program->setUniform("viewProjection", viewProjection);
}

void TetPainter::setViewUniform(const glm::mat4 & view)
{
    m_program->setUniform("view", view);
}

void TetPainter::setEyeUniform(const glm::vec3 & eye)
{
    m_program->setUniform("eye", eye);
}

void TetPainter::paint(BunchOfTetsDrawable & drawable, const glm::mat4 & modelMatrix)
{
    m_program->setUniform("model", modelMatrix);
    drawable.paint();
}

void TetPainter::initialize()
{
    m_program = new glow::Program();

    glow::Shader * m_fragShader = glowutils::createShaderFromFile(
        GL_FRAGMENT_SHADER, "data/shaders/tets.frag");
    glow::Shader * m_vertShader = glowutils::createShaderFromFile(
        GL_VERTEX_SHADER, "data/shaders/tets.vert");
    m_program->attach(m_vertShader, m_fragShader);
    m_program->link();


    m_program->setUniform("vertices", 0);
    m_program->setUniform("normals", 1);
    m_program->setUniform("matrices", 2);
}
