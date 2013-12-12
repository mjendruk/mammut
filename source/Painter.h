#pragma once

#include <QList>

#include "AbstractPainter.h"

class Drawable;

namespace glow
{
    class Program;
    class Shader;
}

class Painter : public AbstractPainter
{
public:
    Painter(Drawable & cuboidDrawable);
    virtual ~Painter();

    virtual bool initialize();
    
    virtual void paint(Drawable & drawable, const glm::mat4 & modelMatrix);

    void setNearFarUniform(const glm::vec2 & nearFar);
    void setViewProjectionUniform(const glm::mat4 & viewProjection);
    void setViewUniform(const glm::mat4 & view);
    void setEyeUniform(const glm::vec3 & eye);
    
    virtual void update(const QList<glow::Program *> & programs);

protected:
    Drawable & m_cuboidDrawable;
    glow::Program * m_program;

};
