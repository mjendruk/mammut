#pragma once

#include <QList>
#include <QSize>

class QStringList;
class QKeyEvent;

namespace glow
{
    class Program;
}

class Camera;


class AbstractPainter
{
public:
    AbstractPainter();
    virtual ~AbstractPainter();

    void setCamera(Camera * camera);
    Camera * camera();

    virtual bool initialize() = 0;

    virtual void paint(float timef) = 0;

    virtual void update() = 0;
    virtual void update(const QList<glow::Program *> & programs) = 0;

protected:
    Camera * m_camera;

};
