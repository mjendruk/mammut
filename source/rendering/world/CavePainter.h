#pragma once

#include <QList>

#include "AbstractPainter.h"

class DrawableInterface;

namespace glow
{
    class Program;
    class Shader;
}

class CavePainter : public AbstractPainter
{
public:
    CavePainter();
    virtual ~CavePainter();

    virtual bool initialize();
    
    virtual void paint(DrawableInterface & drawable, const glm::mat4 & modelMatrix);

    void setNearFarUniform(const glm::vec2 & nearFar);
    void setViewProjectionUniform(const glm::mat4 & viewProjection);
    void setViewUniform(const glm::mat4 & view);
    void setEyeUniform(const glm::vec3 & eye);
    
    virtual void update(const QList<glow::Program *> & programs);

protected:
    glow::Program * m_program;

};
