
#pragma once

#include <QMatrix4x4>

//#include <glow/Shader.h>

#include "AbstractPainter.h"

class QOpenGLShader;

namespace glow
{
    class Program;
    class Shader;
    class Icosahedron;
}

class btDiscreteDynamicsWorld;
class btRigidBody;

class Quad;


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

protected:
    glow::Program * m_program;

    glow::Shader * m_fragShader;
    glow::Shader * m_vertShader;
    
    QVector<Quad *> m_quads;
    
    btDiscreteDynamicsWorld * m_dynamicsWorld;
    btRigidBody * m_fallRigidBody;
};
