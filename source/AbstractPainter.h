#pragma once

#include <glm/glm.hpp>

#include <QList>
#include <QSize>

class QStringList;
class QKeyEvent;

namespace glow
{
    class Program;
}

class RenderCamera;
class Drawable;


class AbstractPainter
{
public:
    AbstractPainter();
    virtual ~AbstractPainter();

    void setCamera(RenderCamera * camera);
    RenderCamera * camera();

    virtual bool initialize() = 0;

    virtual void paint(Drawable & drawable, glm::mat4 & modelMatrix) = 0;

    virtual void update() = 0;
    virtual void update(const QList<glow::Program *> & programs) = 0;

protected:
    RenderCamera * m_camera;

};
