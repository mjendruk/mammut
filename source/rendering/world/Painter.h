#pragma once

#include <QList>
#include <glow/ref_ptr.h>
#include "AbstractPainter.h"

class IDrawable;

namespace glow
{
    class Program;
    class Shader;
}

class Painter : public AbstractPainter
{
public:
    Painter();
    virtual ~Painter();

    virtual bool initialize();
    
    virtual void paint(IDrawable & drawable, const glm::mat4 & modelMatrix);

    void setNearFarUniform(const glm::vec2 & nearFar);
    void setViewProjectionUniform(const glm::mat4 & viewProjection);
    void setViewUniform(const glm::mat4 & view);
    void setEyeUniform(const glm::vec3 & eye);
    
    virtual void update(const QList<glow::Program *> & programs);

protected:
    glow::ref_ptr<glow::Program> m_program;

};
