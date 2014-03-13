#pragma once

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
    
    virtual void paint(DrawableInterface & drawable,
                       const glm::mat4 & modelMatrix,
                       const glm::mat4 & prevModelMatrix,
                       bool containsBoost);

    void setViewProjectionUniforms(const glm::mat4 & viewProjection,
                                   const glm::mat4 & prevViewProjection);
    
    void setViewUniform(const glm::mat4 & view);

protected:
    virtual void initialize();

protected:
    glow::ref_ptr<glow::Program> m_program;

};
