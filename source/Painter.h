#pragma once

#include <QList>
#include <QVector>

//#include <glow/Shader.h>

#include "AbstractPainter.h"

class QOpenGLShader;

class Quad;

namespace glow
{
    class Program;
    class Shader;
    class Icosahedron;
}


class Painter : public AbstractPainter
{
public:
    Painter();
    virtual ~Painter();

    virtual bool initialize();
    
    virtual void paint(float timef);
    virtual void resize(
        int width
    ,   int height);

    virtual void update();
    virtual void update(const QList<glow::Program *> & programs);

	QVector<Quad *> * m_quads;

protected:
    glow::Program * m_program;

    glow::Shader * m_fragShader;
    glow::Shader * m_vertShader;

};
