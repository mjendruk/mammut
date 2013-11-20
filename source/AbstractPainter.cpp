#include "AbstractPainter.h"

#include <cassert>

#include <GL/glew.h>

#include <QStringList>
#include <QKeyEvent>

#include "Camera.h"


AbstractPainter::AbstractPainter()
:   m_camera(nullptr)
{
}
 
AbstractPainter::~AbstractPainter()
{
}

void AbstractPainter::setCamera(Camera * camera)
{
    if (m_camera == camera)
        return;

    m_camera = camera;
    update();
}

Camera * AbstractPainter::camera()
{
    return m_camera;
}