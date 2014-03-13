#pragma once

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
    
    virtual void paint(DrawableInterface & drawable,
                       const glm::mat4 & modelMatrix,
                       const glm::mat4 & prevModelMatrix,
                       bool containsBoost = false);

    void setViewProjectionUniforms(const glm::mat4 & viewProjection,
                                   const glm::mat4 & prevViewProjection);
    
    void setViewUniform(const glm::mat4 & view);

protected:
    virtual void initialize();

protected:
    glow::Program * m_program;
};
