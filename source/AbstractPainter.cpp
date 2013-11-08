
#include <cassert>

#include <QStringList>
#include <QKeyEvent>

#include <glowutils\Camera.h>

#include "AbstractPainter.h"


AbstractPainter::AbstractPainter()
: m_camera(nullptr), m_mode(DEMO_MODE)
{
}
 
AbstractPainter::~AbstractPainter()
{
}

const QStringList& AbstractPainter::extensions() const
{
    static QStringList extensions;
    
    return extensions;
}

void AbstractPainter::show()
{
}

void AbstractPainter::hide()
{
}

void AbstractPainter::setCamera(glow::Camera * camera)
{
    if (m_camera == camera)
        return;

    m_camera = camera;
    update();
}

glow::Camera * AbstractPainter::camera()
{
    return m_camera;
}

const float AbstractPainter::depthAt(const glm::ivec2 & windowCoordinates)
{
    const GLint x(static_cast<GLint>(windowCoordinates.x));
    const GLint y(static_cast<GLint>(windowCoordinates.y));

    assert(m_camera);

    const GLint w(static_cast<GLint>(m_camera->viewport().x));
    const GLint h(static_cast<GLint>(m_camera->viewport().y));

    if (x >= w || y >= h)
        return 1.f;

    GLfloat z;
    glReadPixels(x, h - y - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, reinterpret_cast<void*>(&z));

    return z;
}

const glm::vec3 AbstractPainter::objAt(
    const glm::ivec2 & windowCoordinates)
{
    return objAt(windowCoordinates, depthAt(windowCoordinates), m_camera->viewProjectionInverted());
}

const glm::vec3 AbstractPainter::objAt(
    const glm::ivec2 & windowCoordinates
,   const float depth)
{
    return objAt(windowCoordinates, depth, m_camera->viewProjectionInverted());
}

const glm::vec3 AbstractPainter::objAt(
    const glm::ivec2 & windowCoordinates
,   const float depth
,   const glm::mat4x4 & viewProjectionInverted)
{
    const GLint x(static_cast<GLint>(windowCoordinates.x));
    const GLint y(static_cast<GLint>(windowCoordinates.y));

    // transform viewport to [-1;+1] (including z!)

    const float w = 2.0f / static_cast<float>(m_camera->viewport().x);
    const float h = 2.0f / static_cast<float>(m_camera->viewport().y);

    const glm::vec4 p = glm::vec4(x * w - 1.f, 1.f - y * h, depth * 2.f - 1.f, 1.f);

    // unproject this point back to object space
    glm::vec4 temp = viewProjectionInverted * p;
    return glm::vec3(temp / temp.w);
}

void AbstractPainter::keyPressEvent(QKeyEvent * event)
{
	switch (event->key())
	{
	case Qt::Key_0:
		m_mode = DEMO_MODE;
		break;
	case Qt::Key_1:
		m_mode = EXERCISE_1;
		break;
	case Qt::Key_2:
		m_mode = EXERCISE_2;
		break;
	case Qt::Key_3:
		m_mode = EXERCISE_3;
		break;
	default:
		break;
	}
}
