#include "AbstractPainter.h"

#include <cassert>

#include <GL/glew.h>

#include <QStringList>
#include <QKeyEvent>

#include "RenderCamera.h"


AbstractPainter::AbstractPainter()
:   m_camera(nullptr)
{
}
 
AbstractPainter::~AbstractPainter()
{
}

void AbstractPainter::setCamera(RenderCamera * camera)
{
    if (m_camera == camera)
        return;

    m_camera = camera;
    update();
}

RenderCamera * AbstractPainter::camera()
{
    return m_camera;
}
