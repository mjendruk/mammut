#pragma once

#include <QList>
#include <QSize>

#include <glowutils/AbstractCoordinateProvider.h>

class QStringList;
class QKeyEvent;

namespace glow
{
    class Program;
}

class Camera;


class AbstractPainter : public glow::AbstractCoordinateProvider
{
public:
    AbstractPainter();
    virtual ~AbstractPainter();

    void setCamera(Camera * camera);
    Camera * camera();

    /** return list of mandatory extensions (verify results in fatal if one is
        not supported).
    */
    virtual const QStringList & extensions() const;

    /** this should setup the opengl state and allocate all required static 
        resources (context is 3.2 core for this one).
    */
    virtual bool initialize() = 0;

    /** returns wether m_context is nullptr or not
    */
    bool isValid() const;

    virtual void show();
    virtual void hide();

    virtual void paint(float timef) = 0;
    virtual void resize(int width, int height) = 0;

    virtual void update() = 0;
    virtual void update(const QList<glow::Program *> & programs) = 0;

    // AbstractCoordinateProvider interface

    virtual const float depthAt(const glm::ivec2 & windowCoordinates);

    virtual const glm::vec3 objAt(
        const glm::ivec2 & windowCoordinates);
    const glm::vec3 objAt(
        const glm::ivec2 & windowCoordinates
    ,   const float depth);
    const glm::vec3 objAt(
        const glm::ivec2 & windowCoordinates
    ,   const float depth
    ,   const glm::mat4x4 & viewProjectionInverted);

protected:
    Camera * m_camera;

};
