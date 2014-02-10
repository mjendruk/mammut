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
class DrawableInterface;

class AbstractPainter
{
public:
    AbstractPainter();
    virtual ~AbstractPainter();

    virtual bool initialize() = 0;

    virtual void paint(DrawableInterface & drawable, const glm::mat4 & modelMatrix, const glm::mat4 & prevModelMatrix) = 0;

    virtual void update(const QList<glow::Program *> & programs) = 0;
};
