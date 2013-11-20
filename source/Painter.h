
#pragma once

#include <QList>

#include <glm/glm.hpp>

#include "AbstractPainter.h"

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
    
    virtual void paint(float timef);

    virtual void update();
    virtual void update(const QList<glow::Program *> & programs);

protected:
    bool m_initialized;
    glow::Program * m_program;
    glow::Shader * m_fragShader;
    glow::Shader * m_vertShader;

};
