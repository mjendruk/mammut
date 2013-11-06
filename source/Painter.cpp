
#include <cassert>

#include <GL/glew.h>
#include <qgl.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <glow/Array.h>
#include <glow/Error.h>

#include "Format.h"
#include "Painter.h"


using namespace glow;
using namespace glm;

Painter::Painter()
{
}

Painter::~Painter()
{
}

AbstractContext * Painter::context() const
{
	return nullptr;
}

const bool Painter::initialize(const Format & format)
{
	if (isValid())
		return true;

    // enable fast gl error callbacks

    /*Error::setupDebugOutput();
    Error::setChecking(false);*/


    // init shaders, buffers, and vaos

	glClearColor(1.f, .5f, 1.f, 0.f);

/*
    Shader * vert = Shader::fromFile(GL_VERTEX_SHADER, "data/grid.vert");
    Shader * frag = Shader::fromFile(GL_FRAGMENT_SHADER, "data/grid.frag");

    m_program = new Program();
    m_program->attach(vert, frag);
    m_program->bindFragDataLocation(0, "fragColor");


    Array<vec4> points;
    for (float i = -1.f; i <= 1.f; i += 0.25f)
        points  << vec4(1.f, 0.f, i, 0.f) << vec4(-1.f, 0.f, i, 0.f)
                << vec4(i, 0.f, 1.f, 0.f) << vec4(i, 0.f, -1.f, 0.f);

    m_buffer = new Buffer(GL_ARRAY_BUFFER);
    m_buffer->setData(points, GL_STATIC_DRAW);

    m_vao = new VertexArrayObject;

    auto binding = m_vao->binding(0);
    auto a_vertex = m_program->getAttributeLocation("a_vertex");

    binding->setAttribute(a_vertex);
    binding->setBuffer(m_buffer, 0, sizeof(vec4));
    binding->setFormat(4, GL_FLOAT, GL_FALSE, 0);

    m_vao->enable(a_vertex);*/

	return true;
}

void Painter::paint(const unsigned int targetFBO)
{
	//if (!m_context || !m_context->valid())
	//	return;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
/*
    m_program->use();

    m_vao->bind();
    m_buffer->drawArrays(GL_LINES, 0, 18);
    m_vao->unbind();

    m_program->release();*/

}

void Painter::resize(
	const int width
,	const int height)
{
   /* glViewport(0, 0, width, height);

    const mat4 projection(perspective(40.f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 8.f));

    const vec3 eye(0.f, 1.f,-2.f);
    const vec3 at (0.f, 0.f, 0.f);
    const vec3 up (0.f, 1.f, 0.f);

    const mat4 view(lookAt(eye, at, up));

    m_program->setUniform("modelView", view);
    m_program->setUniform("projection", projection);*/
}
