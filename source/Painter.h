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

    void setTransformUniform(const glm::mat4 & viewProjection);
    virtual void update(const QList<glow::Program *> & programs);

protected:
    Drawable & m_cuboidDrawable;
    glow::Program * m_program;
    glow::Shader * m_fragShader;
    glow::Shader * m_vertShader;

};
