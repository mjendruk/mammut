#pragma once

#include <QList>
#include <glow/ref_ptr.h>
#include "AbstractPainter.h"

class DrawableInterface;

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
    
virtual void paint(DrawableInterface & drawable, const glm::mat4 & modelMatrix, const glm::mat4 & prevModelMatrix);

    void setNearFarUniform(const glm::vec2 & nearFar);
    void setViewProjectionUniforms(const glm::mat4 & viewProjection, const glm::mat4 & prevViewProjection);
    void setViewUniform(const glm::mat4 & view);
    void setEyeUniform(const glm::vec3 & eye);
    
    virtual void update(const QList<glow::Program *> & programs);

protected:
    glow::ref_ptr<glow::Program> m_program;

};
