#include "Painter.h"

#include <cassert>

#include <glow/Program.h>
#include <glow/Shader.h>
#include <glowutils/Icosahedron.h>

#include "FileAssociatedShader.h"
#include "Camera.h"
#include "Quad.h"

Painter::Painter()
:   m_program(nullptr)
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
    glClearColor(1.f, 1.f, 1.f, 0.f);

    m_program = new glow::Program();

    m_fragShader = FileAssociatedShader::getOrCreate(
        GL_FRAGMENT_SHADER, "data/default.frag", *m_program);
    m_vertShader = FileAssociatedShader::getOrCreate(
        GL_VERTEX_SHADER, "data/default.vert", *m_program);
    m_program->link();
    
    return true;
}

void Painter::resize(
    int width
,   int height)
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
		for (Quad * quad : *m_quads) {
			m_program->setUniform("model", quad->matrix());
			quad->draw();
		}
		m_program->release();
	}
}
