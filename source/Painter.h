
#pragma once

#include <QMatrix4x4>

//#include <glow\Shader.h>

#include "AbstractPainter.h"

class QOpenGLShader;
class QOpenGLShaderProgram;



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
    virtual void update(const QList<QOpenGLShaderProgram *> & programs);

protected:
    QOpenGLShaderProgram * m_program;

    QOpenGLShader * m_fragShader;
    QOpenGLShader * m_vertShader;
};
